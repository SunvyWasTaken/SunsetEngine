//
// Created by sunvy on 28/06/2026.
//

#pragma once
#include "SRmGUI_fwd.h"

namespace SRmGUI
{
    using TextureID = uint32_t;

    namespace Forme
    {
        struct Base
        {
            glm::vec2 Position {0, 0};
            glm::vec2 Size {0, 0};
            glm::vec4 Color {1.0f};
        };

        struct Rectangle : Base
        {
            int Radius = 0;
        };
        struct Image : Base
        {
            TextureID texture = 0;
            glm::vec4 Uv{0.f, 1.f, 1.f, 0.f};
        };
        struct Text : Base
        {
            std::string text;
        };

        using Type = std::variant<Rectangle, Image, Text>;
    }

    using FormeDatas = std::vector<Forme::Type>;

    template<class... Ts>
    struct overloads : Ts... { using Ts::operator()...; };

    struct MouseEvent
    {
        enum class Type
        {
            None = 0,
            Press = 1,
            Hold = 2,
            Release = 3,
        };

        glm::ivec2 position{0, 0};
        MouseEvent::Type type = MouseEvent::Type::None;
        uint16_t key = 0;
    };

    struct DragDropPayload
    {
        std::string Type;
        void* Data = nullptr;
    };

    struct DragState
    {
        bool IsMouseDown = false;
        bool IsDragging = false;

        WidgetPtr PressedWidget = nullptr;
        WidgetPtr SourceWidget = nullptr;
        WidgetPtr TargetWidget = nullptr;

        glm::ivec2 PressMousePos{0, 0};
        glm::ivec2 CurrentMousePos{0, 0};

        DragDropPayload DragPayload;
    };
}
