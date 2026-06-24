//
// Created by sunvy on 24/06/2026.
//

#pragma once

#include "Widget.h"

namespace Sunset
{
    class UIImage : public Widget
    {
    public:
        UIImage(uint32_t texture);

        void Paint(UIRenderList& out) override;

    private:
        uint32_t m_Texture;
        glm::vec4 m_UV{0.f, 0.f, 1.f, 1.f};
        glm::vec4 m_Color{1.f};
    };
} // Sunset