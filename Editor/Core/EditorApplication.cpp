//
// Created by sunvy on 06/07/2026.
//

#include "EditorApplication.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Core/GameModuleLoader.h"
#include "SaveSystem/SaveSystem.h"

namespace Sunset
{
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
    }

    void EditorApplication::EndFrame()
    {
        if (!PrintScreen::Get().empty())
        {
            ImGui::Begin("Stats", nullptr);
            for (const auto& it : PrintScreen::Get())
            {
                ImGui::Text("%s", it.c_str());
            }
            ImGui::End();
        }
        const auto& profiles = ProfileData::Get();
        if (!profiles.empty())
        {
            if (ImGui::Begin("Profiling", nullptr, ImGuiWindowFlags_HorizontalScrollbar))
            {
                auto first = profiles.front().start;
                auto last = profiles.front().end;
                float labelWidth = 90.0f;
                for (const auto& profile : profiles)
                {
                    if (profile.start < first) first = profile.start;
                    if (profile.end > last) last = profile.end;
                    const float nameWidth = ImGui::CalcTextSize(profile.name.c_str()).x + 16.0f;
                    if (nameWidth > labelWidth) labelWidth = nameWidth;
                }
                if (labelWidth > 220.0f) labelWidth = 220.0f;

                const float frameMs = std::chrono::duration<float, std::milli>(last - first).count();
                ImGui::Text("Frame %.2f ms", frameMs);

                const float availableWidth = ImGui::GetContentRegionAvail().x;
                const float width = availableWidth > 480.0f ? availableWidth : 480.0f;
                const float durationWidth = 82.0f;
                const float padding = 8.0f;
                const float plotX = labelWidth + padding;
                const float plotWidth = width - plotX - durationWidth - padding;
                const float rowHeight = ImGui::GetTextLineHeightWithSpacing() + 6.0f;
                const float rowsHeight = padding * 2.0f + rowHeight * static_cast<float>(profiles.size());
                const float axisHeight = ImGui::GetTextLineHeightWithSpacing() + 4.0f;

                const ImVec2 origin = ImGui::GetCursorScreenPos();
                ImGui::Dummy(ImVec2(width, rowsHeight + axisHeight));
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                drawList->PushClipRect(origin, ImVec2(origin.x + width, origin.y + rowsHeight + axisHeight), true);
                drawList->AddRect(origin, ImVec2(origin.x + width, origin.y + rowsHeight),
                                  ImGui::GetColorU32(ImGuiCol_Border));

                for (size_t i = 0; i < profiles.size(); ++i)
                {
                    const auto& profile = profiles[i];
                    const float startMs = std::chrono::duration<float, std::milli>(profile.start - first).count();
                    const float durationMs = std::chrono::duration<float, std::milli>(profile.end - profile.start).count();
                    const float y = origin.y + padding + rowHeight * static_cast<float>(i);
                    const float barStart = origin.x + plotX + (frameMs > 0.0f ? startMs / frameMs * plotWidth : 0.0f);
                    const float barEnd = origin.x + plotX + (frameMs > 0.0f ? (startMs + durationMs) / frameMs * plotWidth : 0.0f);

                    drawList->PushClipRect(ImVec2(origin.x + padding, y),
                                           ImVec2(origin.x + labelWidth, y + rowHeight), true);
                    drawList->AddText(ImVec2(origin.x + padding, y + 3.0f),
                                      ImGui::GetColorU32(ImGuiCol_Text), profile.name.c_str());
                    drawList->PopClipRect();

                    drawList->AddRectFilled(ImVec2(barStart, y + 3.0f),
                                            ImVec2(barEnd, y + rowHeight - 3.0f),
                                            ImGui::GetColorU32(ImGuiCol_PlotHistogram));
                    const std::string durationLabel = std::format("{:.2f} ms", durationMs);
                    drawList->AddText(ImVec2(origin.x + width - durationWidth + padding, y + 3.0f),
                                      ImGui::GetColorU32(ImGuiCol_Text), durationLabel.c_str());
                }

                const float axisY = origin.y + rowsHeight + 2.0f;
                drawList->AddText(ImVec2(origin.x + plotX, axisY),
                                  ImGui::GetColorU32(ImGuiCol_TextDisabled), "0 ms");
                const std::string endLabel = std::format("{:.2f} ms", frameMs);
                drawList->AddText(ImVec2(origin.x + plotX + plotWidth - ImGui::CalcTextSize(endLabel.c_str()).x, axisY),
                                  ImGui::GetColorU32(ImGuiCol_TextDisabled), endLabel.c_str());
                drawList->PopClipRect();
            }
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        Application::EndFrame();
    }

    void EditorApplication::OnWindowReady()
    {
        Application::OnWindowReady();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        SaveSystem::CreateFolder(SAVE_PATH);
        io.IniFilename = SAVE_PATH "imgui.ini";
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_DockingEnable;
        io.FontDefault = io.Fonts->AddFontFromFileTTF(SUNSET_EDITOR_LOCAL_RESOURCES "JetBrainMono/JetBrainsMono-Regular.ttf", 18.f);

        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        constexpr float RoundingVal = 5.f;
        style.WindowRounding = RoundingVal;
        style.ChildRounding = RoundingVal;
        style.FrameRounding = RoundingVal;
        style.PopupRounding = RoundingVal;
        style.GrabRounding = RoundingVal;
        style.FrameBorderSize = 1.f;
        style.WindowMenuButtonPosition = ImGuiDir_None;

        ImVec4* colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.16f, 0.16f, 0.16f, 0.54f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.29f, 0.29f, 0.29f, 0.40f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(1.00f, 0.77f, 0.88f, 0.67f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(1.00f, 0.62f, 0.80f, 1.00f);
        colors[ImGuiCol_CheckboxSelectedBg]     = ImVec4(0.16f, 0.16f, 0.16f, 0.45f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(1.00f, 0.62f, 0.80f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(1.00f, 0.77f, 0.88f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.92f, 0.50f, 0.69f, 1.00f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(1.00f, 0.62f, 0.80f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.83f, 0.42f, 0.60f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.56f, 0.16f, 0.37f, 1.00f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(1.00f, 0.62f, 0.80f, 0.80f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.85f, 0.44f, 0.62f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(1.00f, 0.62f, 0.80f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.85f, 0.44f, 0.62f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(1.00f, 0.62f, 0.80f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(1.00f, 0.62f, 0.80f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.92f, 0.50f, 0.69f, 0.95f);
        colors[ImGuiCol_InputTextCursor]        = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.75f, 0.35f, 0.53f, 1.00f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.66f, 0.25f, 0.45f, 1.00f);
        colors[ImGuiCol_TabSelected]            = ImVec4(0.75f, 0.35f, 0.53f, 1.00f);
        colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TabDimmed]              = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.56f, 0.16f, 0.37f, 1.00f);
        colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
        colors[ImGuiCol_DockingPreview]         = ImVec4(1.00f, 0.77f, 0.88f, 1.00f);
        colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextLink]               = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_TreeLines]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_DragDropTargetBg]       = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_UnsavedMarker]          = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_NavCursor]              = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(GetWindow()), true);
        ImGui_ImplOpenGL3_Init("#version 330");

        if (!loader.Load(SUNSET_EDITOR_GAME_MODULE_PATH, *this))
        {
            throw std::runtime_error("Cannot load game module : " SUNSET_EDITOR_GAME_MODULE_PATH);
        }
    }
} // Sunset
