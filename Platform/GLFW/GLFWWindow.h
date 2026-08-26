//
// Created by sunvy on 16/07/2026.
//

#pragma once

#include "Core/Event.h"
#include "Core/Window.h"

class GLFWwindow;

namespace Sunset
{
    class GraphicsContext;

    class GLFWWindow : public Window
    {
    public:
        explicit GLFWWindow(const WindowSetting& setting);
        ~GLFWWindow() override;

        GLFWWindow(const GLFWWindow&) = delete;
        GLFWWindow& operator=(const GLFWWindow&) = delete;
        GLFWWindow(GLFWWindow&&) = delete;
        GLFWWindow& operator=(GLFWWindow&&) = delete;

        void PollEvents() override;
        void Present() override;

        [[nodiscard]]
        bool ShouldClose() const override;

        void RequestClose() override;

        void SetVSync(bool enabled) override;

        [[nodiscard]]
        bool IsVSync() const override;

        [[nodiscard]]
        glm::ivec2 Size() const override;

        void BindEvent(const std::function<void(const Event::Type&)> &event) override;

        [[nodiscard]]
        void* GetNativeHandle() const override;

    private:
        void Initialize(const WindowSetting& properties);
        void Shutdown();
        void RegisterCallbacks();

    private:
        struct WindowData
        {
            std::string Title{};
            glm::ivec2 Size{};
            bool VSync = false;

            std::function<void(const Event::Type&)> EventCallback;
        };

        GLFWwindow* m_WindowHandle = nullptr;
        // std::unique_ptr<GraphicsContext> m_GraphicsContext;

        WindowData m_Data;
        WindowGraphicsAPI m_GraphicsAPI = WindowGraphicsAPI::OpenGL;

        static std::uint32_t s_WindowCount;
    };
} // Sunset
