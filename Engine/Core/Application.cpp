//
// Created by sunvy on 15/12/2025.
//

#include "Application.h"

#include "ApplicationSetting.h"
#include "Layer.h"
#include "Render/RenderCommande.h"
#include "Render/Renderer.h"

#include <chrono>
#include <deque>

namespace
{
    SunsetEngine::Application* app = nullptr;
    SunsetEngine::ApplicationSetting AppSetting;
    bool IsAppRunning = true;
    SunsetEngine::Renderer* m_Render = nullptr;

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
            PRINTSCREEN("FPS : {}", 1.f/sommeDt);
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

namespace SunsetEngine
{
    Application::Application(const ApplicationSetting& setting)
        : m_LayerStack()
    {
        Log::Init();
        INITLOG("Engine");
        LOG("Engine", info, "App Create");
        app = this;
        AppSetting = setting;
        m_Render = new Renderer();
        m_Render->BindEvent([this](Event::Type& event){ OnEvent(event); });

        InputRegister::Init("SunsetCraft/Sources/Input.json");
    }

    Application::~Application()
    {
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

        while (IsAppRunning && m_Render->Valid())
        {
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            std::chrono::duration<float> dt = now - prev;
            prev = now;

            fpsema.Add(dt.count());
            fpsema.Display();

            {
                HeapTest logic("Logic part");
                for (const auto& layer : m_LayerStack)
                {
                    layer->OnUpdate(dt.count());
                }
            }

            {
                HeapTest Render("Render part");
                RenderCommande::BeginFrame();
                for (auto layer = m_LayerStack.end(); layer != m_LayerStack.begin(); )
                {
                    (*--layer)->OnDraw();
                }
                RenderCommande::EndFrame();
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

    const Application& Application::GetApplication()
    {
        return *app;
    }

    void Application::ResizeWindow(const glm::ivec2& setting)
    {
        AppSetting.WindowSize = setting;
    }

    void* Application::GetWindow()
    {
        return Renderer::Get();
    }
}
