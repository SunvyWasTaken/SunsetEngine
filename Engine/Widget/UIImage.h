//
// Created by sunvy on 24/06/2026.
//

#pragma once

#include "Widget.h"

namespace Sunset
{
    class Texture;

    class UIImage : public Widget
    {
    public:
        UIImage();

        void Paint(UIRenderList& out) override;

        void LoadImage(const std::string_view& fileName);
    private:
        std::shared_ptr<Texture> m_Texture;
        glm::vec4 m_UV{0.f, 0.f, 1.f, 1.f};
        glm::vec4 m_Color{1.f};
    };
} // Sunset
