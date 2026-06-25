//
// Created by sunvy on 24/06/2026.
//

#pragma once

#include "Widget.h"

namespace Sunset
{
    class Texture;
    class UIRenderList;

    class UIButton : public Widget
    {
    public:
        void Paint(UIRenderList& out) override;
    private:
        std::shared_ptr<Texture> m_Texture;
        glm::vec4 UV{0, 0, 1, 1};
        bool m_Pressed = false;
    };
} // Sunset