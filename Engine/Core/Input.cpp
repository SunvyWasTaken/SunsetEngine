//
// Created by sunvy on 15/12/2025.
//

#include "Input.h"

#include "Render/Renderer.h"
#include "Utility/UtilityFunction.h"

#include <GLFW/glfw3.h>

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
    }

    LocalInputSource::~LocalInputSource()
    {
    }

    InputState LocalInputSource::GetInput()
    {
        m_InputState.Update();
        return m_InputState;
    }

    NetworkInputSource::NetworkInputSource()
    {
    }

    NetworkInputSource::~NetworkInputSource()
    {
    }

    InputState NetworkInputSource::GetInput()
    {

    }
}
