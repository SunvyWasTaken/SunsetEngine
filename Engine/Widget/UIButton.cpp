//
// Created by sunvy on 24/06/2026.
//

#include "UIButton.h"

#include "Widget.h"
#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    void UIButton::Paint(UIRenderList &out)
    {
        Widget::Paint(out);

        out.AddImage(m_Bounds.position, m_Bounds.size, {1.f, 1.f, 1.f, 1.f}, m_Texture, UV);
    }
} // Sunset