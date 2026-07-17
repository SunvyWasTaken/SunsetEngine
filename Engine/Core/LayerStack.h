//
// Created by sunvy on 16/12/2025.
//

#pragma once

#include "Layer.h"

namespace Sunset
{
    class Layer;

    class LayerStack final
    {
    public:

        LayerStack() = default;

        ~LayerStack() = default;

        void Clear();

        template <typename T, typename... Args>
        requires std::is_base_of_v<Layer, T>
        void PushOverlay(Args&& ...args)
        {
            m_Layers.emplace(m_Layers.begin() + m_OverlayPosition++, std::make_unique<T>(std::forward<Args>(args)...));
        }

        template<typename T, typename... Args>
        requires std::is_base_of_v<Layer, T>
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

        std::unique_ptr<Layer>& LastOverlay()
        {
            return *(m_Layers.end() - (m_Layers.size() - m_OverlayPosition));
        }

        std::unique_ptr<Layer>& back()
        {
            return m_Layers.back();
        }

    private:
        std::vector<std::unique_ptr<Layer>> m_Layers;
        std::uint32_t m_OverlayPosition = 0;
    };
}
