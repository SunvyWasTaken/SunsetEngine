//
// Created by sunvy on 24/06/2026.
//

#pragma once

namespace Sunset
{
    class UIRenderList;

    struct Rectangle
    {
        glm::ivec2 position {0, 0}, size {1, 1};
        [[nodiscard]]
        bool Contains(const glm::ivec2& pos) const;
    };

    class Widget
    {
    public:
        virtual ~Widget() = default;
        virtual void Update(float dt);
        virtual void ComputeDesiredSize();
        virtual void Arrange(const Rectangle& parentRect);
        virtual void Paint(UIRenderList& renderList);
        virtual Widget* HitTest(const glm::ivec2& mouse);
        void SetVisible(bool visible);
        [[nodiscard]] bool IsVisible() const;
        void SetDesiredSize(const glm::ivec2& desiredSize);
        [[nodiscard]] const glm::ivec2& GetDesiredSize() const;
        [[nodiscard]] const Rectangle& GetBounds() const;

        bool bIsVisible = true;
        Rectangle m_Bounds{};
        glm::ivec2 m_DesiredSize {120, 36};
    };
} // Sunset