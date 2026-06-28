//
// Created by sunvy on 28/06/2026.
//

#pragma once

namespace SRmGUI
{
    namespace Forme
    {
        struct Base;
        struct Rectangle;
        struct Image;
        struct Text;
        using Type = std::variant<Rectangle, Image, Text>;
    }
    using FormeDatas = std::vector<Forme::Type>;

    using WidgetPtr = std::shared_ptr<class Widget>;
    using WidgetWeak = std::weak_ptr<Widget>;

    struct Rect;
}