//
// Created by sunvy on 28/06/2026.
//

#include "Image.h"

#include "../Include/Type.h"

namespace SRmGUI
{
    void Image::Update(float dt)
    {
        Widget::Update(dt);
    }

    void Image::Arrange(const Rect &viewportRect)
    {
        Widget::Arrange(viewportRect);
    }

    void Image::Paint(FormeDatas &out)
    {
        Widget::Paint(out);
        out.emplace_back(Forme::Image{{m_DesireParameter.Position, m_DesireParameter.Size, {1.f, 1.f, 1.f, 1.f}}, m_TextureID});
    }

    void Image::SetImage(TextureID id)
    {
        m_TextureID = id;
    }

    void Image::SetUv(const glm::vec4 &uv)
    {
        m_Uv = uv;
    }
} // SRmGUI