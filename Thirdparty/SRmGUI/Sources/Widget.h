//
// Created by sunvy on 28/06/2026.
//

#pragma once

namespace SRmGUI
{
    using WidgetPtr = std::shared_ptr<class Widget>;
    using WidgetWeak = std::weak_ptr<Widget>;

    class Widget : public std::enable_shared_from_this<Widget>
    {
    public:
        void SetParent(const WidgetPtr& parent);
        WidgetPtr GetSelf();
    protected:
        WidgetWeak m_Parent;
    };
} // SRmGUI