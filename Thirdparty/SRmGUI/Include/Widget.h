//
// Created by sunvy on 28/06/2026.
//

#pragma once

#include "Rect.hpp"
#include "SRmGUI_fwd.h"

namespace SRmGUI
{
    class Widget : public std::enable_shared_from_this<Widget>
    {
    public:
        Widget();
        virtual ~Widget() = default;

        void SetParent(const WidgetPtr& parent);
        WidgetPtr GetSelf();

        virtual void Update(float dt);

        virtual void Arrange(const Rect& viewportRect);

        virtual void Paint(FormeDatas& out);

        virtual void OnMouseMove(glm::vec2 mousePos);

        Rect GetDesireRect() const;

        void SetPosition(const glm::vec2& position);

        void SetSize(const glm::vec2& size);

    protected:
        WidgetWeak m_Parent;
        Rect m_DesireParameter;
        bool m_IsVisible;
    };
} // SRmGUI