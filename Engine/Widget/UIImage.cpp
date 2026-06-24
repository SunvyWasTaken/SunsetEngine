//
// Created by sunvy on 24/06/2026.
//

#include "UIImage.h"

#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    UIImage::UIImage(uint32_t texture)
        : m_Texture(texture)
    {
    }

    void UIImage::Paint(UIRenderList &out)
    {
        if (!bIsVisible)
            return;

        Widget::Paint(out);
        out.AddImage(m_Bounds.position, m_Bounds.size, m_Color, m_Texture, m_UV);
    }
} // Sunset
