//
// Created by sunvy on 27/06/2026.
//

#include "UIText.h"

#include "Render/UI/UIRenderList.h"

namespace Sunset
{
    UIText::UIText(std::string text)
        : m_Text(std::move(text))
    {
    }

    void UIText::ComputeDesiredSize()
    {
        int currentLineLength = 0;
        int maxLineLength = 0;
        int lineCount = m_Text.empty() ? 0 : 1;

        for (const char character : m_Text)
        {
            if (character == '\n')
            {
                maxLineLength = std::max(maxLineLength, currentLineLength);
                currentLineLength = 0;
                ++lineCount;
                continue;
            }

            ++currentLineLength;
        }

        maxLineLength = std::max(maxLineLength, currentLineLength);
        m_DesiredSize.x = maxLineLength > 0 ? (maxLineLength * m_GlyphSize.x) + ((maxLineLength - 1) * (m_GlyphSize.x / 5)) : 0;
        m_DesiredSize.y = lineCount > 0 ? (lineCount * m_GlyphSize.y) + ((lineCount - 1) * (m_GlyphSize.y / 7)) : 0;
    }

    void UIText::Paint(UIRenderList& renderList)
    {
        if (!bIsVisible || m_Text.empty())
            return;

        Widget::Paint(renderList);
        renderList.AddText(m_Text, m_Bounds.position, m_GlyphSize, m_Color);
    }

    void UIText::SetText(std::string text)
    {
        m_Text = std::move(text);
        ComputeDesiredSize();
    }

    const std::string& UIText::GetText() const
    {
        return m_Text;
    }

    void UIText::SetColor(const glm::vec4& color)
    {
        m_Color = color;
    }

    const glm::vec4& UIText::GetColor() const
    {
        return m_Color;
    }

    void UIText::SetGlyphSize(const glm::ivec2& glyphSize)
    {
        m_GlyphSize = glm::max(glyphSize, glm::ivec2{1, 1});
        ComputeDesiredSize();
    }

    const glm::ivec2& UIText::GetGlyphSize() const
    {
        return m_GlyphSize;
    }
} // Sunset