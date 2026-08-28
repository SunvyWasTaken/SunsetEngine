//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "WindowSetting.h"
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

        void SetWindow(std::unique_ptr<Window> window);

        void InitializeWindow();

        void Run();

        virtual void OnDestroy();

        virtual void OnWindowReady();

        virtual void BeginFrame();

        void Update(float deltatime);

        void Render();

        virtual void EndFrame();

        void OnEvent(const Event::Type& event);

#pragma region Layer
        template <typename T, typename ...Args>
        void PushOverlay(Args&& ...args)
        {
            m_LayerStack.PushOverlay<T>(std::forward<Args>(args)...);
            m_LayerStack.LastOverlay()->Init();
        }

        /// Push layer will create and add the layer to the Layer Stack
        template <typename T, typename ...Args>
        void PushLayer(Args&&... args)
        {
            m_LayerStack.PushLayer<T>(std::forward<Args>(args)...);
            m_LayerStack.back()->Init();
        }

        /// Add Layer will just add an existing layer to the layer stack
        /// If you intend to create a layer and send it directly to the LayerStack Use "PushLayer"
        void AddLayer(Layer* layer)
        {
            m_LayerStack.AddLayer(layer);
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
#pragma endregion // Layer

        static const WindowSetting& GetSetting();

        static Application& GetApplication();

        static void* GetWindow();

        static bool IsHeadless();

        static void CloseApplication();

    private:
        LayerStack m_LayerStack;
        std::unique_ptr<Window> m_Window;
        std::vector<std::function<void()>> m_CommandBuffer;
    };
}
