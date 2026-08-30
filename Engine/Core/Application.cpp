//
// Created by sunvy on 15/12/2025.
//

#include "Application.h"

#include "Audio/AudioSystem.h"
#include "GameFramework/Components/InputComponent.h"
#include "GameFramework/World/Entity.h"
#include "GameFramework/World/World.h"
#include "Layer.h"
#include "Network/NetworkService.h"
#include "Render/Core/Renderer.h"
#include "Render/Core/RenderAPI.h"
#include "Window.h"
#include "WindowSetting.h"

namespace
{
    Sunset::Application* app = nullptr;
    Sunset::WindowSetting AppSetting;
    bool IsAppRunning = true;

    void ResizeWindow(const glm::ivec2& setting)
    {
        AppSetting.WindowSize = setting;
        Sunset::Application::GetAPI()->SetViewport(setting);
    }
}

namespace Sunset
{
    Application::Application(const WindowSetting& setting)
        : m_LayerStack()
        , m_Window(nullptr)
        , m_Renderer(nullptr)
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

    void Application::SetRenderAPI(std::unique_ptr<RenderAPI> API)
    {
        m_Renderer = std::make_unique<Renderer>(std::move(API));
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

            BeginFrame();

            Update(dt.count());

            if (!AppSetting.Headless)
                Render();

            EndFrame();

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
        m_Renderer.reset();
        m_Window.reset();
    }

    void Application::BeginFrame()
    {
        if (m_Window)
        {
            m_Window->PollEvents();
        }

        GetAPI()->BeginFrame();
    }

    void Application::Update(const float deltatime)
    {
        SS_PROFILE_SCOPE("Logic part");
        if (!IsHeadless())
        {
            AudioSystem::Update();
        }

        if (NetworkService::IsInitialized())
            NetworkService::Get().Update(deltatime);

        for (const auto& layer : m_LayerStack)
        {
            layer->OnUpdate(deltatime);
        }
    }

    void Application::Render()
    {
        SS_PROFILE_SCOPE("Render part");

        for (auto layer = m_LayerStack.end(); layer != m_LayerStack.begin(); )
            (*--layer)->OnDraw(m_Renderer.get());
    }

    void Application::EndFrame()
    {
        GetAPI()->EndFrame();

        m_Window->Present();
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

    RenderAPI * Application::GetAPI()
    {
        if (!app)
            return nullptr;

        return app->m_Renderer->m_API.get();
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
