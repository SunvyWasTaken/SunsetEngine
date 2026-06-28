//
// Created by sunvy on 24/06/2026.
//

#include "UIRenderList.h"

#include "Render/Texture.h"

namespace Sunset
{
    void UIRenderList::Clear()
    {
        m_UIDrawList.clear();
    }

    void UIRenderList::AddRectangle(const glm::ivec2 &position, const glm::ivec2 &size, const glm::vec4 &color, int radius)
    {
        m_UIDrawList.emplace_back(UIDraw::Rectangle{position, size, color, radius});
    }

    void UIRenderList::AddImage(const glm::ivec2 &position, const glm::ivec2 &size, const glm::vec4 &color,
        const std::shared_ptr<Texture>& texture, const glm::vec4 &uv)
    {
        m_UIDrawList.emplace_back(UIDraw::Image{position, size, color, texture, uv});
    }

    void UIRenderList::AddText(const std::string &text, const glm::ivec2 &position, const glm::ivec2 &size,
        const glm::vec4 &color)
    {
        m_UIDrawList.emplace_back(UIDraw::Text{position, size, color, text});
    }

    bool UIRenderList::IsEmpty() const
    {
        return m_UIDrawList.empty();
    }
} // Sunset
