//
// Created by sunvy on 15/12/2025.
//

#pragma once

#include "GameFramework/Controller.h"

namespace Sunset
{
    struct Event
    {
        enum class Action
        {
            None,
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
            unsigned int button;
            int Scroll;
            Action action;
            glm::ivec2 position {0, 0};
        };

        using Type = std::variant<KeyEvent, MouseEvent>;
    };

    struct InputRegister
    {
        // Take a path to a .json with the key in it.
        static void Init(const std::string_view& Path);
        static bool OnEvent(const Event::Type& event);
        [[nodiscard]] static glm::vec2 GetMouseDelta();
        static bool IsKeyPress(const std::string_view& name);
        static void RegisterAction(const std::string_view& name, const std::function<bool(const Event::Action&)>& func);
    };

    class InputState final
    {
    public:
        InputState();
        ~InputState();
        void Update();
        std::unordered_map<std::string, Event::Type> RegisteredInputs;

        Event::Type operator[](const std::string_view& name) const;
    };

    class IInputSource
    {
    public:
        virtual ~IInputSource() = default;

        virtual InputState GetInput() = 0;
    };

    class LocalInputSource : public IInputSource
    {
    public:
        LocalInputSource();
        ~LocalInputSource() override;

        [[nodiscard]] InputState GetInput() override;

    private:
        InputState m_InputState;
    };

    class NetworkInputSource : public IInputSource
    {
    public:
        explicit NetworkInputSource(PeerId peer);
        ~NetworkInputSource() override;

        [[nodiscard]] InputState GetInput() override;

    private:
        InputState m_InputState;
        PeerId m_peerId;
    };
}
