//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_APPLICATION_H
#define SUNSETCRAFT_APPLICATION_H

#include "Input.h"
#include "LayerStack.h"

namespace SunsetEngine
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

        template <typename T, typename ...Args>
        requires std::is_base_of_v<SunsetEngine::Layer, T>
        void PushLayer(Args&&... args)
        {
            m_LayerStack.PushLayer<T>(std::forward<Args>(args)...);
        }

        void AddLayer(Layer* layer)
        {
            m_LayerStack.AddLayer(layer);
        }

        static const ApplicationSetting& GetSetting();
        static const Application& GetApplication();
        static void ResizeWindow(const glm::ivec2& setting);
        static void* GetWindow();

    private:

        SunsetEngine::LayerStack m_LayerStack;
    };
}

#endif //SUNSETCRAFT_APPLICATION_H