#include "FilesystemRenameDialog.h"

#include <cstring>
#include <imgui.h>
#include <system_error>

namespace Sunset
{
    void FilesystemRenameDialog::Open(const std::filesystem::path& path, const bool isDirectory)
    {
        m_Target = path;
        m_Extension = isDirectory ? std::filesystem::path{} : path.extension();
        m_Error.clear();

        const std::string name = (isDirectory ? path.filename() : path.stem()).string();
        m_NameBuffer.fill('\0');
        std::strncpy(m_NameBuffer.data(), name.c_str(), m_NameBuffer.size() - 1);
        m_ShouldOpen = true;
    }

    std::optional<RenamedPath> FilesystemRenameDialog::Render()
    {
        if (m_ShouldOpen)
        {
            ImGui::OpenPopup("Rename item##FilesystemRenameDialog");
            m_ShouldOpen = false;
        }

        std::optional<RenamedPath> renamed;
        if (!ImGui::BeginPopupModal("Rename item##FilesystemRenameDialog", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            return renamed;

        ImGui::InputText("Name", m_NameBuffer.data(), m_NameBuffer.size());
        if (!m_Extension.empty())
            ImGui::Text("Extension: %s", m_Extension.string().c_str());

        if (!m_Error.empty())
            ImGui::TextColored(ImVec4(0.95f, 0.25f, 0.25f, 1.0f), "%s", m_Error.c_str());

        if (ImGui::Button("Rename"))
        {
            const std::string name = m_NameBuffer.data();
            if (name.empty() || name == "." || name == ".." || name.find_first_of("/\\") != std::string::npos)
            {
                m_Error = "Enter a valid name without path separators.";
            }
            else
            {
                const std::filesystem::path newPath = m_Target.parent_path() / (name + m_Extension.string());
                if (newPath == m_Target)
                {
                    m_Target.clear();
                    ImGui::CloseCurrentPopup();
                }
                else
                {
                    std::error_code error;
                    const bool exists = std::filesystem::exists(newPath, error);
                    if (error)
                        m_Error = "Could not check the target name: " + error.message();
                    else if (exists)
                        m_Error = "A file or folder already has this name.";
                    else
                    {
                        std::filesystem::rename(m_Target, newPath, error);
                        if (error)
                            m_Error = "Could not rename item: " + error.message();
                        else
                        {
                            renamed = RenamedPath{m_Target, newPath};
                            m_Target.clear();
                            m_Error.clear();
                            ImGui::CloseCurrentPopup();
                        }
                    }
                }
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            m_Target.clear();
            m_Error.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
        return renamed;
    }
}
