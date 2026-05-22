//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_INPUT_H
#define SUNSETCRAFT_INPUT_H

namespace SunsetEngine
{
    struct Event
    {
        enum class Action
        {
            Press,
            Release,
            Hold
        };

        struct KeyEvent
        {
            unsigned int key;
            Action action;
        };

        struct MouseEvent
        {
            int button;
            int Scroll;
            Action action;
        };

        using Type = std::variant<KeyEvent, MouseEvent>;
    };

    struct InputRegister
    {
        // Take a path to a .json with the key in it.
        static void Init(const std::string_view& Path);
        static bool OnEvent(const Event::Type& event);
        static glm::vec2 GetMouseDelta();
        static bool IsKeyPress(const std::string_view& name);
        static void RegisterAction(const std::string_view& name, const std::function<bool(const Event::Action&)>& func);
    };
}

#endif //SUNSETCRAFT_INPUT_H