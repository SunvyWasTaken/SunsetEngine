//
// Created by sunvy on 15/12/2025.
//

#include "Input.h"

#include "Render/Renderer.h"
#include "Utility/UtilityFunction.h"

#include <GLFW/glfw3.h>

#include "Network/NetworkService.h"

namespace
{
    glm::vec2 lastMousePosition = glm::vec2(0.0f, 0.0f);
    std::map<unsigned int, bool> bIsMouseButtonClick;

    std::unordered_map<std::string, unsigned int> keyMap;

    std::unordered_map<unsigned int, std::function<bool(const Sunset::Event::Action&)>> ActionRegister;

    void ProcessInputs(const nlohmann::json& inputs)
    {
        if (inputs.empty())
            return;

        for (const auto& input : inputs)
        {
            if (!input.contains("name") || !input.contains("key"))
                continue;

            const auto& keyJson = input["key"];

            std::string name = input["name"].get<std::string>();

            unsigned int key = 0;

            if (keyJson.is_number())
                key = keyJson.get<unsigned int>();
            else if (keyJson.is_string())
                key = static_cast<unsigned int>(keyJson.get_ref<const std::string&>()[0]);
            else
                continue;

            keyMap.emplace(name, key);
        }
    }

    struct NetworkInputMessage
    {
        static constexpr Sunset::ChannelId ChannelId = 1;
        uint32_t Buttons = 0;
    };

    enum InputButton : uint32_t
    {
        Input_Forward = 1 << 0,
        Input_Backward = 1 << 1,
        Input_Left = 1 << 2,
        Input_Right = 1 << 3,
    };

    std::unordered_map<Sunset::PeerId, NetworkInputMessage> networkInputs;
    bool networkInputHandlerRegistered = false;

    bool IsPressed(const Sunset::Event::Type& action)
    {
        return std::visit(overloads{
            [](const Sunset::Event::KeyEvent& event)
            {
                return event.action == Sunset::Event::Action::Press || event.action == Sunset::Event::Action::Hold;
            },
            [](const Sunset::Event::MouseEvent& event)
            {
                return event.action == Sunset::Event::Action::Press || event.action == Sunset::Event::Action::Hold;
            }
        }, action);
    }

    void SetAction(Sunset::Event::Type& action, Sunset::Event::Action state)
    {
        std::visit(overloads{
            [state](Sunset::Event::KeyEvent& event)
            {
                event.action = state;
            },
            [state](Sunset::Event::MouseEvent& event)
            {
                event.action = state;
            }
        }, action);
    }

    void SetInputState(Sunset::InputState& inputState, const std::string& name, bool pressed)
    {
        const auto input = inputState.RegisteredInputs.find(name);
        if (input == inputState.RegisteredInputs.end())
            return;

        SetAction(input->second, pressed ? Sunset::Event::Action::Press : Sunset::Event::Action::Release);
    }

    uint32_t BuildNetworkButtons(const Sunset::InputState& inputState)
    {
        uint32_t buttons = 0;

        const auto addButton = [&](const std::string& name, InputButton button)
        {
            const auto input = inputState.RegisteredInputs.find(name);
            if (input != inputState.RegisteredInputs.end() && IsPressed(input->second))
                buttons |= button;
        };

        addButton("Forward", Input_Forward);
        addButton("Backward", Input_Backward);
        addButton("Left", Input_Left);
        addButton("Right", Input_Right);

        return buttons;
    }

    void ApplyNetworkButtons(Sunset::InputState& inputState, uint32_t buttons)
    {
        SetInputState(inputState, "Forward", (buttons & Input_Forward) != 0);
        SetInputState(inputState, "Backward", (buttons & Input_Backward) != 0);
        SetInputState(inputState, "Left", (buttons & Input_Left) != 0);
        SetInputState(inputState, "Right", (buttons & Input_Right) != 0);
    }

    void EnsureNetworkInputHandler()
    {
        if (networkInputHandlerRegistered)
            return;

        Sunset::NetworkService::Get().RegisterMessage<NetworkInputMessage>(1);
        Sunset::NetworkService::Get().RegisterHandler<NetworkInputMessage>([](Sunset::PeerId peer, const NetworkInputMessage& msg)
        {
            networkInputs[peer] = msg;
        });

        networkInputHandlerRegistered = true;
    }
}

namespace Sunset
{
    void InputRegister::Init(const std::string_view& Path)
    {
        if (Path.empty())
            return;

        nlohmann::json j;

        if (!UtilityFunction::DoesFileExist(Path))
            UtilityFunction::SaveJson(Path, j);
        else
            UtilityFunction::LoadJson(Path, j);

        ProcessInputs(j);
    }

    bool InputRegister::OnEvent(const Event::Type& event)
    {
        return std::visit(overloads{
        [](const Event::KeyEvent& event)
        {
            if (ActionRegister.contains(event.key))
            {
                return ActionRegister[event.key](event.action);
            }
            return false;
        }, [](const Event::MouseEvent& event)
        {
            if (ActionRegister.contains(event.button))
            {
                return ActionRegister[event.button](event.action);
            }
            return false;
        }}, event);
    }

    glm::vec2 InputRegister::GetMouseDelta()
    {
        double x, y;
        glfwGetCursorPos(static_cast<GLFWwindow*>(Renderer::Get()), &x, &y);
        glm::vec2 delta = glm::vec2(x, y) - lastMousePosition;
        lastMousePosition = glm::vec2(x, y);
        return delta;
    }

    bool InputRegister::IsKeyPress(const std::string_view& name)
    {
        if (!keyMap.contains(name.data()))
            return false;
        return glfwGetKey(static_cast<GLFWwindow*>(Renderer::Get()), keyMap[name.data()]);
    }

    void InputRegister::RegisterAction(const std::string_view& name,
        const std::function<bool(const Event::Action&)>& func)
    {
        if (!keyMap.contains(name.data()))
        {
            LOG("Engine", error, "The register Input isn't valid {}", name);
            return;
        }

        ActionRegister.emplace(keyMap[name.data()], func);
    }

    InputState::InputState()
    {
        // Load a json for all the input.
        constexpr std::string_view Path{SAVE_PATH "Inputs.json"};

        nlohmann::json j;

        if (!UtilityFunction::DoesFileExist(Path))
            UtilityFunction::SaveJson(Path, j);
        else
            UtilityFunction::LoadJson(Path, j);

        if (j.empty())
            return;

        for (const auto& input : j)
        {
            if (!input.contains("name") || !input.contains("key"))
                continue;

            const auto& keyJson = input["key"];

            std::string name = input["name"].get<std::string>();

            unsigned int key = 0;

            if (keyJson.is_number())
                key = keyJson.get<unsigned int>();
            else if (keyJson.is_string())
                key = static_cast<unsigned int>(keyJson.get_ref<const std::string&>()[0]);
            else
                continue;

            if (const auto& type = input["type"].get<unsigned int>(); type == 0)
                RegisteredInputs.emplace(name, Event::KeyEvent{key, Event::Action::Release});
            else
                RegisteredInputs.emplace(name, Event::MouseEvent{key, 0, Event::Action::Release});
        }

    }

    InputState::~InputState()
    {
    }

    void InputState::Update()
    {
        SS_PROFILE_FUNCTION();
        for (auto& [key, action] : RegisteredInputs)
        {
            std::visit(overloads{
                [&](Event::KeyEvent& event)
                {
                    if (glfwGetKey(static_cast<GLFWwindow*>(Renderer::Get()), event.key) == GLFW_PRESS)
                        event.action = Event::Action::Press;
                    else if (glfwGetKey(static_cast<GLFWwindow*>(Renderer::Get()), event.key) == GLFW_RELEASE)
                        event.action = Event::Action::Release;
                },
                [&](Event::MouseEvent& event)
                {
                    if (glfwGetMouseButton(static_cast<GLFWwindow*>(Renderer::Get()), event.button) == GLFW_PRESS)
                        event.action = Event::Action::Press;
                    else if (glfwGetMouseButton(static_cast<GLFWwindow*>(Renderer::Get()), event.button) == GLFW_RELEASE)
                        event.action = Event::Action::Release;
                }
            }, action);
        }
        NetworkInputMessage msg;
        msg.Buttons = BuildNetworkButtons(*this);
        NetworkService::Get().Broadcast(msg, DeliveryType::Unreliable);
    }

    Event::Type InputState::operator[](const std::string_view &name) const
    {
        if (!RegisteredInputs.contains(name.data()))
            return {};

        return RegisteredInputs.at(name.data());
    }

    LocalInputSource::LocalInputSource()
        : m_InputState()
    {
        EnsureNetworkInputHandler();
    }

    LocalInputSource::~LocalInputSource()
    {
    }

    InputState LocalInputSource::GetInput()
    {
        m_InputState.Update();
        return m_InputState;
    }

    NetworkInputSource::NetworkInputSource(PeerId peer)
        : m_InputState()
        , m_peerId(peer)
    {
        EnsureNetworkInputHandler();
    }

    NetworkInputSource::~NetworkInputSource()
    {
    }

    InputState NetworkInputSource::GetInput()
    {
        if (const auto input = networkInputs.find(m_peerId); input != networkInputs.end())
            ApplyNetworkButtons(m_InputState, input->second.Buttons);
        return m_InputState;
    }
}
