//
// Created by sunvy on 15/12/2025.
//

#include "Application.h"

#include "Audio/AudioSystem.h"
#include "GameFramework/Components/InputComponent.h"
#include "GameFramework/World/Entity.h"
#include "GameFramework/World/World.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Network/NetworkService.h"
#include "Render/Core/RenderCommand.h"
#include "Window.h"
#include "WindowSetting.h"

namespace
{
    Sunset::Application* app = nullptr;
    Sunset::WindowSetting AppSetting;
    bool IsAppRunning = true;

    struct EMA
    {
        std::vector<float> dts;
        size_t currentIndex = 0;
        const size_t nbrFrame = 60;

        EMA()
        {
            dts.reserve(nbrFrame);
        }

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
            if (dts.size() < nbrFrame)
                dts.emplace_back(dt);
            else
            {
                dts[currentIndex++] = dt;
                if (currentIndex >= nbrFrame)
                    currentIndex = 0;
            }

        }
    }fpsema;

    void ResizeWindow(const glm::ivec2& setting)
    {
        AppSetting.WindowSize = setting;
        Sunset::RenderCommand::SetViewport(setting);
    }
}

namespace Sunset
{
    Application::Application(const WindowSetting& setting)
        : m_LayerStack()
        , m_Window(nullptr)
        , m_GameInstance(std::make_unique<GameInstance>())
        , m_CommandBuffer()
    {
        Log::Init();
        INITLOG("Engine");
        LOG("Engine", info, "App Create");
        app = this;
        AppSetting = setting;
        IsAppRunning = true;
    }

    Application::~Application()
    {
        OnDestroy();

        app = nullptr;

        LOG("Engine", info, "App Destroy");

        Log::Shutdown();
    }

    void Application::SetWindow(std::unique_ptr<Window> window)
    {
        m_Window = std::move(window);
        m_Window->BindEvent([this](const Event::Type& event) { OnEvent(event); });
    }

    void Application::InitializeWindow()
    {
        if (!m_Window)
            throw std::runtime_error("Application window has not been configured");
        OnWindowReady();
    }

    void Application::Run()
    {
        std::chrono::steady_clock::time_point prev = std::chrono::steady_clock::now();

        while (IsAppRunning && (AppSetting.Headless || (m_Window && !m_Window->ShouldClose())))
        {
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            std::chrono::duration<float> dt = now - prev;
            prev = now;

            if (m_Window)
            {
                m_GameInstance->m_ActiveWorld->BeginInput();
                m_Window->PollEvents();
            }

            if (!AppSetting.Headless)
            {
                fpsema.Add(dt.count());
                fpsema.Display();
            }

            if (NetworkService::IsInitialized())
                NetworkService::Get().Update(dt.count());

            {
                SS_PROFILE_SCOPE("Logic part");
                m_GameInstance->Update(dt.count());
                for (const auto& layer : m_LayerStack)
                {
                    layer->OnUpdate(dt.count());
                }
            }

            if (!AppSetting.Headless)
            {
                AudioSystem::Update();

                SS_PROFILE_SCOPE("Render part");
                BeginFrame();
                for (auto layer = m_LayerStack.end(); layer != m_LayerStack.begin(); )
                    (*--layer)->OnDraw();

                EndFrame();
                m_Window->Present();
            }

            if (!m_CommandBuffer.empty())
            {
                for (const auto& func : m_CommandBuffer)
                {
                    func();
                }
                m_CommandBuffer.clear();
            }

            PrintScreen::Clear();
            ProfileData::Free();

            if (AppSetting.Headless && AppSetting.HeadlessTickRate > 0.0f)
            {
                const auto targetFrameTime = std::chrono::duration<float>(1.0f / AppSetting.HeadlessTickRate);
                const auto elapsed = std::chrono::steady_clock::now() - now;
                if (elapsed < targetFrameTime)
                    std::this_thread::sleep_for(targetFrameTime - elapsed);
            }
        }
    }

    void Application::OnDestroy()
    {
        AudioSystem::Shutdown();

        m_CommandBuffer.clear();
        m_LayerStack.Clear();
        m_GameInstance.reset();
        RenderCommand::Shutdown();
        m_Window.reset();
    }

    void Application::BeginFrame()
    {
        RenderCommand::BeginFrame();
    }

    void Application::EndFrame()
    {
        RenderCommand::EndFrame();
    }

    void Application::OnWindowReady()
    {
        AudioSystem::Init();
    }

    void Application::OnEvent(const Event::Type& event)
    {
        if (std::holds_alternative<Event::Window>(event))
        {
            const auto&[size] = std::get<Event::Window>(event);
            ResizeWindow(size);
        }
        for (const auto& layer : m_LayerStack)
        {
            if (layer->OnEvent(event))
                return;
        }
        m_GameInstance->m_ActiveWorld->Each<InputComponent>([&](const Entity& entity, InputComponent& comp)
        {
            comp.OnEvent(event);
        });
    }

    const WindowSetting& Application::GetSetting()
    {
        return AppSetting;
    }

    Application& Application::GetApplication()
    {
        return *app;
    }

    void* Application::GetWindow()
    {
        if (!app)
            return nullptr;

        return app->m_Window->GetNativeHandle();
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
