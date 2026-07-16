//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "WindowSetting.h"
#include "Input.h"
#include "LayerStack.h"

namespace Sunset
{
    class Window;
    struct WindowSetting;
    class Layer;

    class Application
    {
    public:
        explicit Application(const WindowSetting& setting = WindowSetting{});
        virtual ~Application();

        void Run();

        virtual void OnDestroy();
        virtual void BeginFrame();
        virtual void EndFrame();

        void OnEvent(const Event::Type& event);

        template <typename T, typename ...Args>
        void PushOverlay(Args&& ...args)
        {
            m_LayerStack.PushOverlay<T>(std::forward<Args>(args)...);
            m_LayerStack.LastOverlay()->SetAppContext(m_GameInstance.get());
            m_LayerStack.LastOverlay()->Init();
        }

        /// Push layer will create and add the layer to the Layer Stack
        template <typename T, typename ...Args>
        void PushLayer(Args&&... args)
        {
            m_LayerStack.PushLayer<T>(std::forward<Args>(args)...);
            m_LayerStack.back()->SetAppContext(m_GameInstance.get());
            m_LayerStack.back()->Init();
        }

        /// Add Layer will just add an existing layer to the layer stack
        /// If you intend to create a layer and send it directly to the LayerStack Use "PushLayer"
        void AddLayer(Layer* layer)
        {
            m_LayerStack.AddLayer(layer);
            m_LayerStack.back()->SetAppContext(m_GameInstance.get());
            m_LayerStack.back()->Init();
        }

        template <typename T, typename ...Args>
        void LoadOverlay(Args&& ...args)
        {
            m_CommandBuffer.emplace_back([&]()->void
            {
                PushOverlay<T>(std::forward<Args>(args)...);
            });
        }

        template <typename T, typename ...Args>
        void LoadLayer(Args&& ...args)
        {
            using ArgsTuple = std::tuple<std::decay_t<Args>...>;
            m_CommandBuffer.emplace_back([this, storedArgs = ArgsTuple(std::forward<Args>(args)...)]() mutable ->void
            {
                std::apply(
                    [this](auto&... values)
                    {
                        PushLayer<T>(std::move(values)...);
                    }, storedArgs);
            });
        }

        void ClearLayer()
        {
            m_CommandBuffer.emplace_back([&]()
            {
               m_LayerStack.Clear();
            });
        }

        template <typename T, typename ...Args>
        void SetGameInstance(Args&&... args)
        {
            m_GameInstance = std::make_unique<T>(std::forward<Args>(args)...);
        }

        static const WindowSetting& GetSetting();
        static Application& GetApplication();
        static void ResizeWindow(const glm::ivec2& setting);
        static void* GetWindow();
        static bool IsHeadless();

        static void CloseApplication();

    private:
        LayerStack m_LayerStack;
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<GameInstance> m_GameInstance;
        std::vector<std::function<void()>> m_CommandBuffer;
    };
}
