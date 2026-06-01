//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_APPLICATION_H
#define SUNSETCRAFT_APPLICATION_H

#include "Input.h"
#include "LayerStack.h"

namespace Sunset
{
    struct ApplicationSetting;
    class Layer;

    class Application
    {
    public:
        explicit Application(const ApplicationSetting& setting);
        virtual ~Application();

        void Run();

        void OnEvent(Event::Type& event);

        /// Push layer will create and add the layer to the Layer Stack
        template <typename T, typename ...Args>
        requires std::is_base_of_v<Layer, T>
        void PushLayer(Args&&... args)
        {
            m_LayerStack.PushLayer<T>(std::forward<Args>(args)...);
        }

        /// Add Layer will just add a existing layer to the layer stack
        /// If you intend to create a layer and send it directly to the LayerStack Use "PushLayer"
        void AddLayer(Layer* layer)
        {
            m_LayerStack.AddLayer(layer);
        }

        static const ApplicationSetting& GetSetting();
        static const Application& GetApplication();
        static void ResizeWindow(const glm::ivec2& setting);
        static void* GetWindow();

    private:

        LayerStack m_LayerStack;
    };
}

#endif //SUNSETCRAFT_APPLICATION_H