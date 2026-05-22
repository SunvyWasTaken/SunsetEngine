//
// Created by sunvy on 11/01/2026.
//

#include "HorizontalBox.h"

namespace SunsetEngine
{
    void HorizontalBox::Draw() const
    {
        const_cast<HorizontalBox*>(this)->Rebuild();

        for (auto& child : m_Children)
            child->Draw();
    }

    void HorizontalBox::Clear()
    {
        m_Children.clear();
    }

    void HorizontalBox::Reserve(size_t size)
    {
        m_Children.reserve(size);
    }

    void HorizontalBox::SetPadding(const glm::ivec2& padding)
    {
        m_Padding = padding;
        bIsDirty = true;
    }

    void HorizontalBox::SetAnchor(const glm::vec2& val)
    {
        m_Anchor = val;
        bIsDirty = true;
    }

    std::shared_ptr<Slate> & HorizontalBox::operator[](const uint8_t index)
    {
        bIsDirty = true;
        return m_Children[index];
    }

    void HorizontalBox::AddChild(const std::shared_ptr<Slate>& child)
    {
        m_Children.emplace_back(child);
        bIsDirty = true;
    }

    std::vector<std::shared_ptr<Slate>>::iterator HorizontalBox::begin()
    {
        return m_Children.begin();
    }

    std::vector<std::shared_ptr<Slate>>::iterator HorizontalBox::end()
    {
        return m_Children.end();
    }

    void HorizontalBox::Rebuild()
    {
        if (!bIsDirty)
            return;

        m_Size = {0, 0};

        for (auto& child : m_Children)
        {
            m_Size.x += child->GetSize().x + m_Padding.x;
        }

        const size_t HalfSize = m_Size.x / 2;
        size_t CurrentPosition = m_Position.x - HalfSize;

        for (auto & i : m_Children)
        {
            size_t x = 0, y = 0;

            x = CurrentPosition + i->GetSize().x / 2;
            y = m_Position.y + (i->GetSize().y / 2) * m_Anchor.y;

            i->SetPosition({x, y});
            CurrentPosition += i->GetSize().x + m_Padding.x;
        }

        bIsDirty = false;
    }
}
