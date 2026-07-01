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

        virtual bool OnMouseEvent(uint8_t type, uint8_t key);

        Rect GetDesireRect() const;

        void SetPosition(const glm::vec2& position);

        void SetSize(const glm::vec2& size);

        void SetVisibility(bool visible);

        void SetPadding(const glm::vec4& padding);

    protected:
        WidgetWeak m_Parent;
        Rect m_DesireParameter;
        bool m_IsVisible;
        // Haut/Droite/Bas/Gauche
        glm::vec4 m_Padding;
    };

    template<typename T>
    class WidgetBuilder;

    template<typename T>
    WidgetBuilder<T> SNew()
    {
        return WidgetBuilder<T>();
    }

    template <typename T>
    WidgetBuilder<T> SNewAssign(std::shared_ptr<T>& obj)
    {
        auto truc = WidgetBuilder<T>();
        obj = truc.ToShared();
        return truc;
    }

    template<typename T, typename Derived>
    class WidgetBuilderBase
    {
    public:
        WidgetBuilderBase()
            : m_Widget(std::make_shared<T>())
        {
        }

        std::shared_ptr<T> ToShared() const
        {
            return m_Widget;
        }

        operator std::shared_ptr<T>() const
        {
            return m_Widget;
        }

        Derived& Position(glm::ivec2 position)
        {
            m_Widget->SetPosition(position);
            return static_cast<Derived&>(*this);
        }

        Derived& Size(glm::ivec2 size)
        {
            m_Widget->SetSize(size);
            return static_cast<Derived&>(*this);
        }

        Derived& Visibility(bool visible)
        {
            m_Widget->SetVisibility(visible);
            return static_cast<Derived&>(*this);
        }

        Derived& Padding(const glm::vec4& padding)
        {
            m_Widget->SetPadding(padding);
            return static_cast<Derived&>(*this);
        }

        template<typename ChildT>
        Derived& Child(const WidgetBuilder<ChildT>& child)
        {
            m_Widget->AddChild(child.ToShared());
            return static_cast<Derived&>(*this);
        }

    protected:
        std::shared_ptr<T> m_Widget;
    };

    template<typename T>
    class WidgetBuilder : public WidgetBuilderBase<T, WidgetBuilder<T>>
    {};
} // SRmGUI