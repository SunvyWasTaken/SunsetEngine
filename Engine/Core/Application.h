//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "Input.h"
#include "LayerStack.h"

namespace Sunset
{
    class Render;
    class UIRender;
    struct ApplicationSetting;
    class Layer;

    class Application
    {
    public:
        explicit Application(const ApplicationSetting& setting);
        virtual ~Application();

        void Run();

        void OnEvent(Event::Type& event);

        template <typename T, typename ...Args>
        void PushOverlay(Args&& ...args)
        {
            m_LayerStack.PushOverlay<T>(std::forward<Args>(args)...);
        }

        /// Push layer will create and add the layer to the Layer Stack
        template <typename T, typename ...Args>
        void PushLayer(Args&&... args)
        {
            m_LayerStack.PushLayer<T>(std::forward<Args>(args)...);
        }

        /// Add Layer will just add an existing layer to the layer stack
        /// If you intend to create a layer and send it directly to the LayerStack Use "PushLayer"
        void AddLayer(Layer* layer)
        {
            m_LayerStack.AddLayer(layer);
        }

        template <typename T, typename ...Args>
        void LoadOverlay(Args&& ...args)
        {
            m_CommandBuffer.emplace_back([&]()->void
            {
                m_LayerStack.PushOverlay<T>(std::forward<Args>(args)...);
            });
        }

        template <typename T, typename ...Args>
        void LoadLayer(Args&& ...args)
        {
            m_CommandBuffer.emplace_back([&]()->void
            {
                m_LayerStack.PushLayer<T>(std::forward<Args>(args)...);
            });
        }

        void ClearLayer()
        {
            m_CommandBuffer.emplace_back([&]()
            {
               m_LayerStack.Clear();
            });
        }

        static const ApplicationSetting& GetSetting();
        static Application& GetApplication();
        static void ResizeWindow(const glm::ivec2& setting);
        static void* GetWindow();
        static bool IsHeadless();

        static void CloseApplication();

    private:
        LayerStack m_LayerStack;
        std::vector<std::function<void()>> m_CommandBuffer;
        std::unique_ptr<UIRender> m_UIRender;
        std::unique_ptr<Render> m_Render;
    };
}
