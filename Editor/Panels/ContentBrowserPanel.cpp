//
// Created by sunvy on 01/09/2026.
//

#include "ContentBrowserPanel.h"

#include <filesystem>
#include <imgui.h>

namespace
{
    const std::filesystem::path CONTENT_BROWSER_PATH = CONTENT_PATH;
    std::filesystem::path currentPath = CONTENT_BROWSER_PATH;
}

namespace Sunset
{
    void ContentBrowserPanel::OnImGuiRender()
    {
        ImGui::Begin("Content");
        if (currentPath != CONTENT_BROWSER_PATH)
        {
            if (ImGui::Button("<-"))
            {
                currentPath = CONTENT_BROWSER_PATH;
            }
        }
        for (auto& file : std::filesystem::directory_iterator(currentPath))
        {
            if (file.is_directory())
            {
                auto fileName = file.path().filename().string();
                if (ImGui::Button(fileName.c_str()))
                {
                    currentPath = file.path();
                }
            }
            else
            {
                auto fileName = file.path().filename().string();
                ImGui::Text(fileName.c_str());
            }
        }
        ImGui::End();
    }
} // Sunset