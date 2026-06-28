//
// Created by sunvy on 24/06/2026.
//

#include "UIImage.h"

#include "Render/Texture.h"
#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    UIImage::UIImage()
        : m_Texture(std::make_shared<Texture>())
    {
    }

    void UIImage::ComputeDesiredSize()
    {
        LeafWidget::ComputeDesiredSize();
    }

    void UIImage::Arrange(const Rectangle &parentRect)
    {
        m_Bounds.position = {0, 0};
        m_Bounds.size = {720, 50};
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
        m_Texture->Reset();
        m_Texture->LoadImage(fileName);
    }
} // Sunset
