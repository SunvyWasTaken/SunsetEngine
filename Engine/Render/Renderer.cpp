//
// Created by sunvy on 15/12/2025.
//

#include "Renderer.h"

#include "Core/Application.h"
#include "Core/ApplicationSetting.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Input.h"

namespace
{
    GLFWwindow* m_Window = nullptr;

    std::function<void(SunsetEngine::Event::Type&)> EventCallback;

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

    GLFWwindow* CreateWindow(const SunsetEngine::ApplicationSetting& setting)
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

    SunsetEngine::Event::Action ItoA(const int action)
    {
        if (action == 0)
            return SunsetEngine::Event::Action::Release;

        if (action == 1)
            return SunsetEngine::Event::Action::Press;

        if (action == 2)
            return SunsetEngine::Event::Action::Hold;

        return SunsetEngine::Event::Action::Release;
    }

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        SunsetEngine::Event::Type event = SunsetEngine::Event::KeyEvent{static_cast<unsigned int>(key), ItoA(action)};
        EventCallback(event);
    }

    void CursorPositionCallback(GLFWwindow* window, double x, double y)
    {
        // SunsetEngine::Event::Type event = SunsetEngine::Event::MouseEvent{x, y};
        // EventCallback(event);
    }

    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        SunsetEngine::Event::Type event = SunsetEngine::Event::MouseEvent{button, 0, ItoA(action)};
        EventCallback(event);
    }

    void CursorScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        SunsetEngine::Event::Type event = SunsetEngine::Event::MouseEvent{0, static_cast<int>(yoffset), ItoA(1)};
        EventCallback(event);
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        SunsetEngine::Application::ResizeWindow({width, height});
    }
}

namespace SunsetEngine
{
    Renderer::Renderer()
    {
        LOG("Engine", info, "Render Create")
        const ApplicationSetting& setting = Application::GetSetting();
        m_Window = CreateWindow(setting);
        if (m_Window == NULL)
        {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_Window);

        glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);

        glfwSetKeyCallback(m_Window, KeyCallback);

        glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);

        glfwSetCursorPosCallback(m_Window, CursorPositionCallback);

        glfwSetScrollCallback(m_Window, CursorScrollCallback);

    #ifdef NDEBUG
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    Renderer::~Renderer()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(m_Window);
        glfwTerminate();
        LOG("Engine", info, "Render Destroy")
    }

    void Renderer::BindEvent(std::function<void(Event::Type&)> func)
    {
        EventCallback = func;
    }

    bool Renderer::Valid() const
    {
        return !glfwWindowShouldClose(m_Window);
    }

    void* Renderer::Get()
    {
        return m_Window;
    }
}
