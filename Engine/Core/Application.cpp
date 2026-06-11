//
// Created by sunvy on 15/12/2025.
//

#include "Application.h"

#include "ApplicationSetting.h"
#include "Layer.h"
#include "BaseObject/BaseCube.h"
#include "Network/NetworkService.h"
#include "Render/RenderCommande.h"
#include "Render/Renderer.h"

namespace
{
    Sunset::Application* app = nullptr;
    Sunset::ApplicationSetting AppSetting;
    bool IsAppRunning = true;
    Sunset::Renderer* m_Render = nullptr;

    struct EMA
    {
        std::deque<float> dts;

        void Display() const
        {
            float sommeDt = 0;
            for (const auto dt : dts)
            {
                sommeDt += dt;
            }
            sommeDt /= dts.size();
            PRINTSCREEN("FPS : {}", floor(1.f/sommeDt));
        }

        void Add(const float dt)
        {
            if (dts.size() >= 180)
            {
                dts.pop_front();
            }
            dts.push_back(dt);
        }
    };

    EMA fpsema;
}

namespace Sunset
{
    Application::Application(const ApplicationSetting& setting)
        : m_LayerStack()
    {
        Log::Init();
        INITLOG("Engine");
        LOG("Engine", info, "App Create");
        app = this;
        AppSetting = setting;
        IsAppRunning = true;
        if (!AppSetting.Headless)
        {
            m_Render = new Renderer();
            m_Render->BindEvent([this](Event::Type& event){ OnEvent(event); });
            InputRegister::Init(SAVE_PATH "Input.json");
        }
        else
        {
            LOG("Engine", info, "App Create in headless mode");
        }
    }

    Application::~Application()
    {
        DeleteBaseCube();
        m_LayerStack.Clear();

        delete m_Render;
        m_Render = nullptr;

        app = nullptr;

        LOG("Engine", info, "App Destroy");

        Log::Shutdown();
    }

    void Application::Run()
    {
        std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();

        while (IsAppRunning && (AppSetting.Headless || (m_Render && m_Render->Valid())))
        {
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            std::chrono::duration<float> dt = now - prev;
            prev = now;

            fpsema.Add(dt.count());
            if (!AppSetting.Headless)
                fpsema.Display();

            if (NetworkService::IsInitialized())
                NetworkService::Get().Update(dt.count());
            {
                SS_PROFILE_SCOPE("Logic part");
                for (const auto& layer : m_LayerStack)
                {
                    layer->OnUpdate(dt.count());
                }
            }

            if (!AppSetting.Headless)
            {
                SS_PROFILE_SCOPE("Render part");
                RenderCommande::BeginFrame();
                for (auto layer = m_LayerStack.end(); layer != m_LayerStack.begin(); )
                {
                    (*--layer)->OnDraw();
                }
                RenderCommande::EndFrame();
            }

            if (!m_CommandBuffer.empty())
            {
                for (const auto& func : m_CommandBuffer)
                {
                    func();
                }
                m_CommandBuffer.clear();
            }

            if (AppSetting.Headless && AppSetting.HeadlessTickRate > 0.0f)
            {
                const auto targetFrameTime = std::chrono::duration<float>(1.0f / AppSetting.HeadlessTickRate);
                const auto elapsed = std::chrono::steady_clock::now() - now;
                if (elapsed < targetFrameTime)
                    std::this_thread::sleep_for(targetFrameTime - elapsed);
            }
        }
    }

    void Application::OnEvent(Event::Type& event)
    {
        if (InputRegister::OnEvent(event))
            return;

        for (const auto& layer : m_LayerStack)
        {
            if (layer->OnEvent(event))
                return;
        }
    }

    const ApplicationSetting& Application::GetSetting()
    {
        return AppSetting;
    }

    Application& Application::GetApplication()
    {
        return *app;
    }

    void Application::ResizeWindow(const glm::ivec2& setting)
    {
        AppSetting.WindowSize = setting;
    }

    void* Application::GetWindow()
    {
        if (!m_Render)
            return nullptr;

        return Renderer::Get();
    }

    bool Application::IsHeadless()
    {
        return AppSetting.Headless;
    }

    void Application::CloseApplication()
    {
        IsAppRunning = false;
    }
}
