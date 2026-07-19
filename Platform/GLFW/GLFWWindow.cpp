//
// Created by sunvy on 16/07/2026.
//

#include "GLFWWindow.h"

#include <GLFW/glfw3.h>

#include "Render/Core/GraphicsContext.h"

namespace
{
    void GlfwErrorCallback(int errorCode, const char* description)
    {
        std::fprintf(
            stderr,
            "GLFW error [%d]: %s\n",
            errorCode,
            description != nullptr ? description : "Unknown error");
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

    Sunset::Event::ButtonAction FromGLFWAction(const int action)
    {
        switch (action)
        {
            case GLFW_PRESS: return Sunset::Event::ButtonAction::Press;
            case GLFW_REPEAT: return Sunset::Event::ButtonAction::Repeat;
            default: return Sunset::Event::ButtonAction::Release;
        }
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
}

namespace Sunset
{
    std::uint32_t GLFWWindow::s_WindowCount = 0;

    GLFWWindow::GLFWWindow(const WindowSetting &setting)
        : m_WindowHandle(nullptr)
        , m_GraphicsContext(nullptr)
    {
        Initialize(setting);
    }

    void GLFWWindow::Initialize(const WindowSetting &properties)
    {
        if (s_WindowCount == 0)
        {
            glfwSetErrorCallback(GlfwErrorCallback);

            if (glfwInit() != GLFW_TRUE)
                throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwDefaultWindowHints();

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifndef NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    #endif

        m_WindowHandle = glfwCreateWindow(static_cast<int>(properties.WindowSize.x), static_cast<int>(properties.WindowSize.y), properties.WindowTitle.data(), nullptr, nullptr);

        if (m_WindowHandle == nullptr)
        {
            if (s_WindowCount == 0)
                glfwTerminate();

            throw std::runtime_error("Failed to create GLFW window");
        }

        ++s_WindowCount;

        m_Data.Title = properties.WindowTitle.data();
        m_Data.Size = properties.WindowSize;
        m_Data.VSync = properties.vSync;

        glfwSetWindowUserPointer(m_WindowHandle, &m_Data);

        // m_GraphicsContext = std::make_unique<OpenGLContext>(m_WindowHandle);
        //
        // m_GraphicsContext->Init();

        RegisterCallbacks();
        SetVSync(m_Data.VSync);
    }

    GLFWWindow::~GLFWWindow()
    {
        Shutdown();
    }

    void GLFWWindow::Shutdown()
    {
        m_GraphicsContext.reset();

        if (m_WindowHandle != nullptr)
        {
            glfwDestroyWindow(m_WindowHandle);
            m_WindowHandle = nullptr;

            --s_WindowCount;
        }

        if (s_WindowCount == 0)
            glfwTerminate();
    }

    void GLFWWindow::PollEvents()
    {
        glfwPollEvents();
    }

    void GLFWWindow::Present()
    {
        glfwSwapBuffers(m_WindowHandle);
    }

    bool GLFWWindow::ShouldClose() const
    {
        return glfwWindowShouldClose(m_WindowHandle) == GLFW_TRUE;
    }

    void GLFWWindow::RequestClose()
    {
        glfwSetWindowShouldClose(m_WindowHandle, GLFW_TRUE);
    }

    void GLFWWindow::SetVSync(bool enabled)
    {
        glfwMakeContextCurrent(m_WindowHandle);
        glfwSwapInterval(enabled ? 1 : 0);

        m_Data.VSync = enabled;
    }

    bool GLFWWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

    glm::ivec2 GLFWWindow::Size() const
    {
        return m_Data.Size;
    }

    void GLFWWindow::BindEvent(const std::function<void(const Event::Type&)> &event)
    {
        m_Data.EventCallback = event;
    }

    void * GLFWWindow::GetNativeHandle() const
    {
        return m_WindowHandle;
    }

    void GLFWWindow::RegisterCallbacks()
    {
        glfwSetFramebufferSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.Size = { width > 0 ? width : 0, height > 0 ? height : 0 };
            data.EventCallback(Event::Window{data.Size});
        });

        glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.EventCallback(Sunset::Event::Keyboard{FromGLFWKey(key), FromGLFWAction(action), FromGLFWMods(mods), scancode});
        });

        glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* window, int button, int action, int mods)
        {
            double x = 0.0, y = 0.0;
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            glfwGetCursorPos(window, &x, &y);
            data.EventCallback(Event::MouseButton{FromGLFWMouseButton(button),
            {static_cast<float>(x), static_cast<float>(y)},
            FromGLFWAction(action),
            FromGLFWMods(mods)});
        });

        glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* window, double xpos, double ypos)
        {
            static glm::f64vec2 previousPosition{0.f, 0.f};
            const glm::f64vec2 position{xpos, ypos};
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.EventCallback(Event::MouseMove{position, position - previousPosition});
            previousPosition = position;
        });

        glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            double x = 0.0, y = 0.0;
            glfwGetCursorPos(window, &x, &y);
            auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.EventCallback(Event::MouseScroll{{xoffset, yoffset}, {x, y}});
        });
    }
} // Sunset