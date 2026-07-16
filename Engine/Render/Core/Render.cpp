//
// Created by sunvy on 15/12/2025.
//

#include "Render.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Application.h"
#include "Core/ApplicationSetting.h"
#include "SRmGUI.h"
#include "SRmGUI_Opengl.h"
#include "Core/Event.h"

namespace
{
    GLFWwindow* m_Window = nullptr;

    std::function<void(Sunset::Event::Type&)> EventCallback;
    std::array<bool, GLFW_JOYSTICK_LAST + 1> ConnectedGamepads{};
    std::array<std::array<unsigned char, GLFW_GAMEPAD_BUTTON_LAST + 1>, GLFW_JOYSTICK_LAST + 1> PreviousGamepadButtons{};
    std::array<std::array<float, GLFW_GAMEPAD_AXIS_LAST + 1>, GLFW_JOYSTICK_LAST + 1> PreviousGamepadAxes{};
    glm::f64vec2 previousPosition{};

    void SendEvent(Sunset::Event::Type event)
    {
        if (EventCallback)
            EventCallback(event);
    }

    void GLAPIENTRY OpenGLDebugCallback(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
    {
        // Ignore les messages non critiques si tu veux
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
            return;

        const char* severityStr =
            severity == GL_DEBUG_SEVERITY_HIGH ? "HIGH" :
            severity == GL_DEBUG_SEVERITY_MEDIUM ? "MEDIUM" :
            severity == GL_DEBUG_SEVERITY_LOW ? "LOW" :
            "NOTIFICATION";

        LOG("OpenGL", error, "[{}] {}", severityStr, message);
    }

    GLFWwindow* CreateWin(const Sunset::ApplicationSetting& setting)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #endif

        return glfwCreateWindow(setting.WindowSize.x, setting.WindowSize.y, setting.WindowTitle.data(), NULL, NULL);
    }

    Sunset::Key FromGLFWKey(int key)
     {
         switch (key)
         {
             case GLFW_KEY_A: return Sunset::Key::A;
             case GLFW_KEY_B: return Sunset::Key::B;
             case GLFW_KEY_C: return Sunset::Key::C;
             case GLFW_KEY_D: return Sunset::Key::D;
             case GLFW_KEY_E: return Sunset::Key::E;
             case GLFW_KEY_F: return Sunset::Key::F;
             case GLFW_KEY_G: return Sunset::Key::G;
             case GLFW_KEY_H: return Sunset::Key::H;
             case GLFW_KEY_I: return Sunset::Key::I;
             case GLFW_KEY_J: return Sunset::Key::J;
             case GLFW_KEY_K: return Sunset::Key::K;
             case GLFW_KEY_L: return Sunset::Key::L;
             case GLFW_KEY_M: return Sunset::Key::M;
             case GLFW_KEY_N: return Sunset::Key::N;
             case GLFW_KEY_O: return Sunset::Key::O;
             case GLFW_KEY_P: return Sunset::Key::P;
             case GLFW_KEY_Q: return Sunset::Key::Q;
             case GLFW_KEY_R: return Sunset::Key::R;
             case GLFW_KEY_S: return Sunset::Key::S;
             case GLFW_KEY_T: return Sunset::Key::T;
             case GLFW_KEY_U: return Sunset::Key::U;
             case GLFW_KEY_V: return Sunset::Key::V;
             case GLFW_KEY_W: return Sunset::Key::W;
             case GLFW_KEY_X: return Sunset::Key::X;
             case GLFW_KEY_Y: return Sunset::Key::Y;
             case GLFW_KEY_Z: return Sunset::Key::Z;

             case GLFW_KEY_SPACE: return Sunset::Key::Space;
             case GLFW_KEY_ESCAPE: return Sunset::Key::Escape;
             case GLFW_KEY_ENTER: return Sunset::Key::Enter;
             case GLFW_KEY_TAB: return Sunset::Key::Tab;
             case GLFW_KEY_BACKSPACE: return Sunset::Key::Backspace;
             case GLFW_KEY_INSERT: return Sunset::Key::Insert;
             case GLFW_KEY_DELETE: return Sunset::Key::Delete;
             case GLFW_KEY_PAGE_UP: return Sunset::Key::PageUp;
             case GLFW_KEY_PAGE_DOWN: return Sunset::Key::PageDown;
             case GLFW_KEY_HOME: return Sunset::Key::Home;
             case GLFW_KEY_END: return Sunset::Key::End;
             case GLFW_KEY_CAPS_LOCK: return Sunset::Key::CapsLock;
             case GLFW_KEY_SCROLL_LOCK: return Sunset::Key::ScrollLock;
             case GLFW_KEY_NUM_LOCK: return Sunset::Key::NumLock;
             case GLFW_KEY_PRINT_SCREEN: return Sunset::Key::PrintScreen;
             case GLFW_KEY_PAUSE: return Sunset::Key::Pause;

             case GLFW_KEY_LEFT: return Sunset::Key::Left;
             case GLFW_KEY_RIGHT: return Sunset::Key::Right;
             case GLFW_KEY_UP: return Sunset::Key::Up;
             case GLFW_KEY_DOWN: return Sunset::Key::Down;

             case GLFW_KEY_LEFT_SHIFT: return Sunset::Key::LeftShift;
             case GLFW_KEY_LEFT_CONTROL: return Sunset::Key::LeftControl;
             case GLFW_KEY_LEFT_ALT: return Sunset::Key::LeftAlt;
             case GLFW_KEY_RIGHT_SHIFT: return Sunset::Key::RightShift;
             case GLFW_KEY_RIGHT_CONTROL: return Sunset::Key::RightControl;
             case GLFW_KEY_RIGHT_ALT: return Sunset::Key::RightAlt;
             case GLFW_KEY_LEFT_SUPER: return Sunset::Key::LeftSuper;
             case GLFW_KEY_RIGHT_SUPER: return Sunset::Key::RightSuper;
             case GLFW_KEY_MENU: return Sunset::Key::Menu;

             case GLFW_KEY_F1: return Sunset::Key::F1;
             case GLFW_KEY_F2: return Sunset::Key::F2;
             case GLFW_KEY_F3: return Sunset::Key::F3;
             case GLFW_KEY_F4: return Sunset::Key::F4;
             case GLFW_KEY_F5: return Sunset::Key::F5;
             case GLFW_KEY_F6: return Sunset::Key::F6;
             case GLFW_KEY_F7: return Sunset::Key::F7;
             case GLFW_KEY_F8: return Sunset::Key::F8;
             case GLFW_KEY_F9: return Sunset::Key::F9;
             case GLFW_KEY_F10: return Sunset::Key::F10;
             case GLFW_KEY_F11: return Sunset::Key::F11;
             case GLFW_KEY_F12: return Sunset::Key::F12;

             default: return Sunset::Key::Unknown;
         }
     }

    Sunset::MouseKey FromGLFWMouseButton(int button)
    {
        switch (button)
        {
            case GLFW_MOUSE_BUTTON_LEFT: return Sunset::MouseKey::Left;
            case GLFW_MOUSE_BUTTON_RIGHT: return Sunset::MouseKey::Right;
            case GLFW_MOUSE_BUTTON_MIDDLE: return Sunset::MouseKey::Middle;
            case GLFW_MOUSE_BUTTON_4: return Sunset::MouseKey::Button_4;
            case GLFW_MOUSE_BUTTON_5: return Sunset::MouseKey::Button_5;
            case GLFW_MOUSE_BUTTON_6: return Sunset::MouseKey::Button_6;
            case GLFW_MOUSE_BUTTON_7: return Sunset::MouseKey::Button_7;
            case GLFW_MOUSE_BUTTON_8: return Sunset::MouseKey::Button_8;
            default: return Sunset::MouseKey::Unknown;
        }
    }

    Sunset::Event::ButtonAction FromGLFWAction(const int action)
    {
        switch (action)
        {
            case GLFW_PRESS: return Sunset::Event::ButtonAction::Press;
            case GLFW_REPEAT: return Sunset::Event::ButtonAction::Repeat;
            default: return Sunset::Event::ButtonAction::Release;
        }
    }

    Sunset::Event::KeyModifiers FromGLFWMods(const int mods)
    {
        return {
            .Shift = (mods & GLFW_MOD_SHIFT) != 0,
            .Control = (mods & GLFW_MOD_CONTROL) != 0,
            .Alt = (mods & GLFW_MOD_ALT) != 0,
            .Super = (mods & GLFW_MOD_SUPER) != 0
        };
    }

    Sunset::GamepadButton FromGLFWGamepadButton(const int button)
    {
        switch (button)
        {
            case GLFW_GAMEPAD_BUTTON_A: return Sunset::GamepadButton::South;
            case GLFW_GAMEPAD_BUTTON_B: return Sunset::GamepadButton::East;
            case GLFW_GAMEPAD_BUTTON_X: return Sunset::GamepadButton::West;
            case GLFW_GAMEPAD_BUTTON_Y: return Sunset::GamepadButton::North;
            case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER: return Sunset::GamepadButton::LeftBumper;
            case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER: return Sunset::GamepadButton::RightBumper;
            case GLFW_GAMEPAD_BUTTON_BACK: return Sunset::GamepadButton::Back;
            case GLFW_GAMEPAD_BUTTON_START: return Sunset::GamepadButton::Start;
            case GLFW_GAMEPAD_BUTTON_GUIDE: return Sunset::GamepadButton::Guide;
            case GLFW_GAMEPAD_BUTTON_LEFT_THUMB: return Sunset::GamepadButton::LeftThumb;
            case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB: return Sunset::GamepadButton::RightThumb;
            case GLFW_GAMEPAD_BUTTON_DPAD_UP: return Sunset::GamepadButton::DPadUp;
            case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT: return Sunset::GamepadButton::DPadRight;
            case GLFW_GAMEPAD_BUTTON_DPAD_DOWN: return Sunset::GamepadButton::DPadDown;
            case GLFW_GAMEPAD_BUTTON_DPAD_LEFT: return Sunset::GamepadButton::DPadLeft;
            default: return Sunset::GamepadButton::Unknown;
        }
    }

    Sunset::GamepadAxis FromGLFWGamepadAxis(const int axis)
    {
        switch (axis)
        {
            case GLFW_GAMEPAD_AXIS_LEFT_X: return Sunset::GamepadAxis::LeftX;
            case GLFW_GAMEPAD_AXIS_LEFT_Y: return Sunset::GamepadAxis::LeftY;
            case GLFW_GAMEPAD_AXIS_RIGHT_X: return Sunset::GamepadAxis::RightX;
            case GLFW_GAMEPAD_AXIS_RIGHT_Y: return Sunset::GamepadAxis::RightY;
            case GLFW_GAMEPAD_AXIS_LEFT_TRIGGER: return Sunset::GamepadAxis::LeftTrigger;
            case GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER: return Sunset::GamepadAxis::RightTrigger;
            default: return Sunset::GamepadAxis::Unknown;
        }
    }

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        SendEvent(Sunset::Event::Keyboard{FromGLFWKey(key), FromGLFWAction(action), FromGLFWMods(mods), scancode});
    }

    void CharCallback(GLFWwindow* window, unsigned int codepoint)
    {
        SendEvent(Sunset::Event::TextInput{static_cast<char32_t>(codepoint)});
    }

    void CursorPositionCallback(GLFWwindow* window, double x, double y)
    {
        const glm::f64vec2 position{x, y};
        SendEvent(Sunset::Event::MouseMove{position, position - previousPosition});
        previousPosition = position;
    }

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        double x = 0.0;
        double y = 0.0;
        glfwGetCursorPos(window, &x, &y);
        SendEvent(Sunset::Event::MouseButton{
            FromGLFWMouseButton(button),
            {static_cast<float>(x), static_cast<float>(y)},
            FromGLFWAction(action),
            FromGLFWMods(mods)
        });
    }

    void CursorScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        double x = 0.0;
        double y = 0.0;
        glfwGetCursorPos(window, &x, &y);
        SendEvent(Sunset::Event::MouseScroll{
            {static_cast<float>(xoffset), static_cast<float>(yoffset)},
            {static_cast<float>(x), static_cast<float>(y)}
        });
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        Sunset::Application::ResizeWindow({width, height});
        if (SRmGUI::HasContext())
            SRmGUI::GetContext().Arrange({{0, 0}, {width, height}});
    }
}

namespace Sunset
{
    Render::Render()
    {
        LOG("Engine", info, "Render Create")
        const ApplicationSetting& setting = Application::GetSetting();
        m_Window = CreateWin(setting);
        if (m_Window == NULL)
        {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_Window);

        glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

        glfwSetKeyCallback(m_Window, KeyCallback);

        glfwSetCharCallback(m_Window, CharCallback);

        glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);

        glfwSetCursorPosCallback(m_Window, CursorPositionCallback);

        glfwSetScrollCallback(m_Window, CursorScrollCallback);

    #ifdef NDEBUG
        // glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #endif

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to initialize GLAD");
        }

#ifndef NDEBUG
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(OpenGLDebugCallback, nullptr);

            // Optionnel : filtrage
            glDebugMessageControl(
                GL_DONT_CARE,
                GL_DONT_CARE,
                GL_DONT_CARE,
                0, nullptr,
                GL_TRUE
            );
        }
#endif

        // glEnable(GL_DEPTH_TEST);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = SAVE_PATH "imgui.ini";
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg].w = 0.2f;

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

#pragma region //SRmGUI
        SRmGUI::Opengl_Init();
#pragma endregion //SRmGUI
    }

    Render::~Render()
    {
        if (m_Window)
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
#pragma region // SRmGUI
            SRmGUI::Opengl_Shutdown();
#pragma endregion
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;
        }
        glfwTerminate();
        LOG("Engine", info, "Render Destroy")
    }

    void Render::BindEvent(std::function<void(Event::Type&)> func)
    {
        EventCallback = func;
    }

    void Render::PollGamepads()
    {
        for (int joystick = GLFW_JOYSTICK_1; joystick <= GLFW_JOYSTICK_LAST; ++joystick)
        {
            const bool connected = glfwJoystickIsGamepad(joystick) == GLFW_TRUE;
            if (ConnectedGamepads[joystick] != connected)
            {
                ConnectedGamepads[joystick] = connected;
                SendEvent(Event::GamepadConnection{static_cast<std::uint8_t>(joystick), connected});
                if (!connected)
                {
                    PreviousGamepadButtons[joystick] = {};
                    PreviousGamepadAxes[joystick] = {};
                    continue;
                }
            }

            if (!connected)
                continue;

            GLFWgamepadstate state;
            if (glfwGetGamepadState(joystick, &state) != GLFW_TRUE)
                continue;

            for (int button = 0; button <= GLFW_GAMEPAD_BUTTON_LAST; ++button)
            {
                const unsigned char previous = PreviousGamepadButtons[joystick][button];
                const unsigned char current = state.buttons[button];
                if (previous == current)
                    continue;

                PreviousGamepadButtons[joystick][button] = current;
                SendEvent(Event::GamepadButton{
                    static_cast<std::uint8_t>(joystick),
                    FromGLFWGamepadButton(button),
                    current == GLFW_PRESS ? Event::ButtonAction::Press : Event::ButtonAction::Release
                });
            }

            for (int axis = 0; axis <= GLFW_GAMEPAD_AXIS_LAST; ++axis)
            {
                const float value = state.axes[axis];
                if (std::abs(PreviousGamepadAxes[joystick][axis] - value) < 0.0001f)
                    continue;

                PreviousGamepadAxes[joystick][axis] = value;
                SendEvent(Event::GamepadAxis{
                    static_cast<std::uint8_t>(joystick),
                    FromGLFWGamepadAxis(axis),
                    value
                });
            }
        }
    }

    bool Render::Valid() const
    {
        return !glfwWindowShouldClose(m_Window);
    }

    void* Render::Get()
    {
        return m_Window;
    }
}
