//
// Created by sunvy on 24/06/2026.
//

#include "UIImage.h"

#include "Render/Texture.h"
#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    UIImage::UIImage()
    {
    }

    void UIImage::Paint(UIRenderList &out)
    {
        if (!bIsVisible)
            return;

        Widget::Paint(out);
        out.AddImage(m_Bounds.position, m_Bounds.size, m_Color, m_Texture, m_UV);
    }

    void UIImage::LoadImage(const std::string_view &fileName)
    {
        m_Texture.reset();
        m_Texture = std::make_shared<Texture>();
        m_Texture->LoadImage(fileName);
    }
} // Sunset
