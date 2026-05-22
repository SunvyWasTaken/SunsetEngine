//
// Created by sunvy on 16/12/2025.
//

#ifndef SUNSETCRAFT_LAYERSTACK_H
#define SUNSETCRAFT_LAYERSTACK_H
#include "Layer.h"

namespace SunsetEngine
{
    class Layer;

    class LayerStack
    {
    public:

        void Clear();

        template<typename T, typename... Args>
        void PushLayer(Args&& ...args)
        {
            m_Layers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        void AddLayer(Layer* layer)
        {
            m_Layers.emplace_back(layer);
        }

        decltype(auto) begin()
        {
            return m_Layers.begin();
        }

        decltype(auto) end()
        {
            return m_Layers.end();
        }

    private:
        std::vector<std::unique_ptr<Layer>> m_Layers;
    };
}

#endif //SUNSETCRAFT_LAYERSTACK_H