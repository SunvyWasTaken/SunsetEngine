//
// Created by sunvy on 06/07/2026.
//

#include "EditorApplication.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Core/GameModuleLoader.h"

namespace Sunset
{
    EditorApplication::EditorApplication(const WindowSetting &setting)
        : Application(setting)
    {

    }

    void EditorApplication::OnDestroy()
    {
        Application::OnDestroy();

        loader.Unload();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void EditorApplication::BeginFrame()
    {
        Application::BeginFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    }

    void EditorApplication::EndFrame()
    {
        Application::EndFrame();

        if (!PrintScreen::Get().empty())
        {
            ImGui::Begin("Stats", nullptr);
            for (const auto& it : PrintScreen::Get())
            {
                ImGui::Text("%s", it.c_str());
            }
            ImGui::End();
        }
        if (!ProfileData::Get().empty())
        {
            ImGui::Begin("Profiling", nullptr);
            for (const auto& it : ProfileData::Get())
            {
                ImGui::Text("%s", it.c_str());
            }
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void EditorApplication::OnWindowReady()
    {
        Application::OnWindowReady();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = SAVE_PATH "imgui.ini";
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_WindowBg].w = 0.2f;

        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(GetWindow()), true);
        ImGui_ImplOpenGL3_Init("#version 330");

        if (!loader.Load(SUNSET_EDITOR_GAME_MODULE_PATH, *this))
        {
            throw std::runtime_error("Cannot load game module : " SUNSET_EDITOR_GAME_MODULE_PATH);
        }
    }
} // Sunset