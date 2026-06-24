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

        Rectangle m_Bounds{};
        glm::ivec2 m_DesiredSize {1, 1};
        bool bIsVisible = true;
    };

    template <typename T, typename ...Args>
    std::shared_ptr<T> SNew(Args&& ...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
} // Sunset