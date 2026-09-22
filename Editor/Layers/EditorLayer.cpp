//
// Created by sunvy on 06/07/2026.
//

#include "EditorLayer.h"

#include <algorithm>
#include <cstring>

#include "Core/Application.h"
#include "Core/FileDialog.h"
#include "GameFramework/World/Entity.h"
#include "GameFramework/World/ScriptEntity.h"
#include "GameFramework/World/World.h"
#include "GameFramework/Components/CameraComponent.h"
#include "GameFramework/Components/InputComponent.h"
#include "GameFramework/Components/NativeScriptComponent.h"
#include "GameFramework/Components/SpriteRenderComponent.h"
#include "GameFramework/Components/TransformComponent.h"
#include "Panels/WorldHierarchyPanel.h"
#include "Render/Resources/RenderTarget.h"
#include "Render/Resources/Texture.h"
#include "Render/Core/Renderer.h"
#include "SaveSystem/SaveSystem.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <ImGuizmo.h>
#include <stb_image.h>

namespace
{
    bool ViewportHover = false;

    struct ImGuiDockspaceArgs
    {
        bool                IsFullscreen = true;
        bool                KeepWindowPadding = false; // Keep WindowPadding to help understand that DockSpace() is a widget inside the window.
        ImGuiDockNodeFlags  DockSpaceFlags  = ImGuiDockNodeFlags_None;
    };

    ImGuizmo::OPERATION gizmoOperation = ImGuizmo::TRANSLATE;

    float Cross(const ImVec2& a, const ImVec2& b, const ImVec2& c)
    {
        return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    }

    bool IsPointInTriangle(const ImVec2& point, const ImVec2& a, const ImVec2& b, const ImVec2& c)
    {
        const float ab = Cross(a, b, point);
        const float bc = Cross(b, c, point);
        const float ca = Cross(c, a, point);

        return (ab >= 0.0f && bc >= 0.0f && ca >= 0.0f) || (ab <= 0.0f && bc <= 0.0f && ca <= 0.0f);
    }

    bool IsPointInQuad(const ImVec2& point, const std::array<ImVec2, 4>& quad)
    {
        return IsPointInTriangle(point, quad[0], quad[1], quad[2])
            || IsPointInTriangle(point, quad[2], quad[3], quad[0]);
    }

    ImVec2 WorldToViewportScreen(const glm::vec3& worldPosition,
                                 const glm::mat4& viewProjection,
                                 const ImVec2& viewportMin,
                                 const ImVec2& viewportSize)
    {
        const glm::vec4 clip = viewProjection * glm::vec4(worldPosition, 1.0f);
        const glm::vec3 ndc = glm::vec3(clip) / clip.w;

        return {
            viewportMin.x + (ndc.x * 0.5f + 0.5f) * viewportSize.x,
            viewportMin.y + (1.0f - (ndc.y * 0.5f + 0.5f)) * viewportSize.y
        };
    }

    std::unique_ptr<Sunset::Texture> LoadTexture(const char* filepath)
    {
        int width = 0;
        int height = 0;
        int channels = 0;
        stbi_uc* data = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);

        if (!data)
            return nullptr;

        Sunset::TextureDescription desc;
        desc.width = static_cast<std::uint32_t>(width);
        desc.height = static_cast<std::uint32_t>(height);
        desc.format = Sunset::TextureFormat::RGBA;
        desc.data = data;

        std::unique_ptr<Sunset::Texture> texture = Sunset::Texture::Create(desc);
        stbi_image_free(data);

        return texture;
    }
}

namespace Sunset
{
    EditorLayer::EditorLayer()
        : m_WorldHierarchy(nullptr)
        , m_Framebuffer(nullptr)
    {
    }

    EditorLayer::~EditorLayer()
    {
    }


    void EditorLayer::Init()
    {
        Layer::Init();
        SetActiveWorld(std::make_shared<World>());
        m_PlayIcon = LoadTexture(SUNSET_EDITOR_LOCAL_RESOURCES "Icons/jouer.png");
        m_PauseIcon = LoadTexture(SUNSET_EDITOR_LOCAL_RESOURCES "Icons/pause.png");
        m_Framebuffer = RenderTarget::Create({1280, 720});
        m_Camera.SetPosition({0, 0, 10});
    }

    void EditorLayer::SetActiveWorld(const std::shared_ptr<World>& world)
    {
        m_World = world;
        if (m_WorldHierarchy)
            m_WorldHierarchy->SetContext(m_World);
        else
            m_WorldHierarchy = std::make_unique<WorldHierarchyPanel>(m_World);

        m_WorldHierarchy->SetSelectedEntity({});
        m_ContentBrowserPanel.SetWorld(m_World);
    }

    void EditorLayer::StartPlayMode()
    {
        if (!std::holds_alternative<EditorState::Edit>(m_EditorState))
            return;

        m_EditorWorld = m_World;
        std::shared_ptr<World> runtimeWorld = m_EditorWorld->Clone();
        SetActiveWorld(runtimeWorld);
        m_World->StartUpdate();
        m_EditorState = EditorState::Play{};
    }

    void EditorLayer::StopPlayMode()
    {
        if (std::holds_alternative<EditorState::Edit>(m_EditorState))
            return;

        m_World->Each<NativeScriptComponent>([](const Entity&, NativeScriptComponent& script)
        {
            script.Stop();
        });

        SetActiveWorld(m_EditorWorld ? m_EditorWorld : std::make_shared<World>());
        m_EditorWorld.reset();
        m_EditorState = EditorState::Edit{};
    }

    void EditorLayer::PausePlayMode()
    {
        if (std::holds_alternative<EditorState::Play>(m_EditorState))
            m_EditorState = EditorState::Pause{};
    }

    void EditorLayer::ResumePlayMode()
    {
        if (std::holds_alternative<EditorState::Pause>(m_EditorState))
            m_EditorState = EditorState::Play{};
    }

    void EditorLayer::OnUpdate(float dt)
    {
        Layer::OnUpdate(dt);

        if (std::holds_alternative<EditorState::Play>(m_EditorState))
        {
            m_World->Update(dt);
            m_World->BeginInput();
        }
    }

    void EditorLayer::OnDraw(Renderer* renderer)
    {
        Layer::OnDraw(renderer);

        RenderWorldViewport(renderer);

        DrawDockspace();

        DrawToolbar();

        DrawViewportPanel();

        DrawPanels();
    }

    bool EditorLayer::OnEvent(const Event::Type &event)
    {
        // m_World->Each<InputComponent>([&](const Entity& entity, InputComponent& comp)
        // {
        //     comp.OnEvent(event);
        // });
        if (std::holds_alternative<EditorState::Play>(m_EditorState))
            m_World->OnEvent(event);
        else if (ViewportHover)
            if (auto* keyboard = std::get_if<Event::Keyboard>(&event))
            {
                if (keyboard->key == Key::W)
                    m_Camera.AddPosition(m_Camera.GetForward());
                else if (keyboard->key == Key::S)
                    m_Camera.AddPosition(-m_Camera.GetForward());
                else if (keyboard->key == Key::A)
                    m_Camera.AddPosition(-m_Camera.GetRight());
                else if (keyboard->key == Key::D)
                    m_Camera.AddPosition(m_Camera.GetRight());
            }

        return Layer::OnEvent(event);
    }

    void EditorLayer::RenderWorldViewport(Renderer *renderer)
    {
        m_Framebuffer->Bind();
        renderer->SetViewport({viewportSize.x, viewportSize.y});
        Camera viewportCamera = m_Camera;
        if (std::holds_alternative<EditorState::Play>(m_EditorState))
        {
            m_World->Each<CameraComponent>([&](const Entity&, const CameraComponent& camera)
            {
                if (camera.Primary)
                {
                    viewportCamera = camera.camera;
                }
            });
        }
        viewportCamera.SetAspectRatio(viewportSize.y > 0.0f ? viewportSize.x / viewportSize.y : 1.0f);
        m_RenderScene.BeginScene(viewportCamera);
        m_BuildRenderScene(*(m_World.get()), m_RenderScene);
        renderer->RenderScene(m_RenderScene);
        m_Framebuffer->UnBind();
        renderer->SetViewport(Application::GetSetting().WindowSize);
    }

    void EditorLayer::DrawDockspace()
    {
        static ImGuiDockspaceArgs args;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        if (args.IsFullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            window_flags |= ImGuiWindowFlags_NoBackground;
        }
        else
        {
            // Floating dockspace
            args.DockSpaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (!args.KeepWindowPadding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        window_flags |= ImGuiWindowFlags_MenuBar;

        ImGui::Begin("SunsetEngine Editor", nullptr, window_flags);

        if (!args.KeepWindowPadding)
            ImGui::PopStyleVar();
        if (args.IsFullscreen)
            ImGui::PopStyleVar(2);

        DrawMenuBar();

        ImGuiID docspace_id = ImGui::GetID("MainDockSpace");
        ImGui::DockSpace(docspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();
    }

    void EditorLayer::DrawMenuBar()
    {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Project")) { /* ... */ }
                const bool canEditWorldFile = std::holds_alternative<EditorState::Edit>(m_EditorState);
                if (ImGui::MenuItem("Open", nullptr, false, canEditWorldFile))
                {
                    const auto worldPath = FileDialog::OpenFile("Open World", CONTENT_PATH, "bin");
                    if (worldPath && SaveSystem::Load(*worldPath, *(m_World.get())))
                        m_ContentBrowserPanel.SetCurrentWorldPath(*worldPath);
                }
                if (ImGui::MenuItem("Save", nullptr, false, canEditWorldFile))
                {
                    const std::filesystem::path& currentWorldPath = m_ContentBrowserPanel.GetCurrentWorldPath();
                    if (!currentWorldPath.empty())
                    {
                        SaveSystem::Save(currentWorldPath, *(m_World.get()));
                    }
                    else
                    {
                        std::ranges::fill(m_SaveWorldAsBuffer, '\0');
                        std::strncpy(m_SaveWorldAsBuffer.data(), "World.bin", m_SaveWorldAsBuffer.size() - 1);
                        m_SaveWorldAsError.clear();
                        m_ShouldOpenSaveWorldAsPopup = true;
                    }
                }
                ImGui::EndMenu();
            }
            // if (ImGui::BeginMenu("Edit")) { /* ... */ ImGui::EndMenu(); }
            // if (ImGui::BeginMenu("View")) { /* ... */ ImGui::EndMenu(); }
            ImGui::EndMenuBar();
        }

        DrawSaveWorldAsPopup();
    }

    void EditorLayer::DrawToolbar()
    {
        ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        if (std::holds_alternative<EditorState::Edit>(m_EditorState))
        {
            if (DrawToolbarImageButton("PlayButton", m_PlayIcon.get(), "Play"))
                StartPlayMode();
        }
        else
        {
            if (std::holds_alternative<EditorState::Play>(m_EditorState))
            {
                if (DrawToolbarImageButton("PauseButton", m_PauseIcon.get(), "Pause"))
                    PausePlayMode();
            }
            else
            {
                if (DrawToolbarImageButton("ResumeButton", m_PlayIcon.get(), "Resume"))
                    ResumePlayMode();
            }

            ImGui::SameLine();
            if (ImGui::Button("Stop"))
                StopPlayMode();
        }

        ImGui::End();
    }

    bool EditorLayer::DrawToolbarImageButton(const char* id, const Texture* icon, const char* fallbackLabel)
    {
        if (!icon)
            return ImGui::Button(fallbackLabel);

        return ImGui::ImageButton(
            id,
            ImTextureRef(static_cast<ImTextureID>(icon->GetRenderID())),
            ImVec2(24.0f, 24.0f)
        );
    }

    void EditorLayer::DrawSaveWorldAsPopup()
    {
        if (m_ShouldOpenSaveWorldAsPopup)
        {
            ImGui::OpenPopup("Save World As");
            m_ShouldOpenSaveWorldAsPopup = false;
        }

        if (ImGui::BeginPopupModal("Save World As", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::InputText("Name", m_SaveWorldAsBuffer.data(), m_SaveWorldAsBuffer.size());

            if (!m_SaveWorldAsError.empty())
                ImGui::TextColored(ImVec4(0.95f, 0.25f, 0.25f, 1.0f), "%s", m_SaveWorldAsError.c_str());

            if (ImGui::Button("Save"))
            {
                std::filesystem::path filename = m_SaveWorldAsBuffer.data();
                if (filename.empty())
                {
                    m_SaveWorldAsError = "World filename cannot be empty.";
                }
                else if (filename.is_absolute() || filename.has_parent_path())
                {
                    m_SaveWorldAsError = "World file must be created directly in Content.";
                }
                else
                {
                    if (!filename.has_extension())
                        filename += ".bin";

                    const std::filesystem::path worldPath = std::filesystem::path(CONTENT_PATH) / filename;
                    if (std::filesystem::exists(worldPath))
                    {
                        m_SaveWorldAsError = "A file already has this name.";
                    }
                    else if (SaveSystem::Save(worldPath, *(m_World.get())))
                    {
                        m_ContentBrowserPanel.SetCurrentWorldPath(worldPath);
                        m_SaveWorldAsError.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    else
                    {
                        m_SaveWorldAsError = "Could not save world file.";
                    }
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                m_SaveWorldAsError.clear();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void EditorLayer::DrawViewportPanel()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");
        ViewportHover = ImGui::IsWindowHovered();
        const auto& AvailableSpace = ImGui::GetContentRegionAvail();
        if (viewportSize.x != AvailableSpace.x || viewportSize.y != AvailableSpace.y)
        {
            viewportSize = AvailableSpace;
            m_Framebuffer = RenderTarget::Create({static_cast<uint32_t>(viewportSize.x), static_cast<uint32_t>(viewportSize.y)});
        }

        m_ViewportMin = ImGui::GetCursorScreenPos();
        m_ViewportMax = ImVec2(m_ViewportMin.x + viewportSize.x, m_ViewportMin.y + viewportSize.y);
        ImGui::Image(m_Framebuffer->GetColorAttachmentRenderID(), viewportSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

        const bool viewportHovered = ImGui::IsItemHovered();
        if (ImGui::IsItemHovered())
        {
            if (ImGui::IsKeyPressed(ImGuiKey_T))
                gizmoOperation = ImGuizmo::TRANSLATE;
            if (ImGui::IsKeyPressed(ImGuiKey_R))
                gizmoOperation = ImGuizmo::ROTATE;
            if (ImGui::IsKeyPressed(ImGuiKey_Y))
                gizmoOperation = ImGuizmo::SCALE;
        }

        DrawGizmo();

        if (viewportHovered
            && ImGui::IsMouseClicked(ImGuiMouseButton_Left)
            && !ImGuizmo::IsOver()
            && !ImGuizmo::IsUsing())
        {
            SelectEntityFromViewport(ImGui::GetMousePos());
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorLayer::DrawGizmo()
    {
        if (!m_WorldHierarchy)
            return;

        Entity selectedEntity = m_WorldHierarchy->GetSelectedEntity();
        if (!selectedEntity)
            return;

        auto* transform = selectedEntity.GetComponent<TransformComponent>();
        if (!transform)
            return;

        ImGuizmo::BeginFrame();
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(m_ViewportMin.x, m_ViewportMin.y, viewportSize.x, viewportSize.y);

        glm::mat4 transformMatrix = transform->GetWorldMatrix();
        const glm::mat4 viewMatrix = m_Camera.GetViewMatrix();
        const float viewportAspect = viewportSize.y > 0.0f ? viewportSize.x / viewportSize.y : 1.0f;
        const glm::mat4 projectionMatrix = m_Camera.GetProjection(viewportAspect);

        ImGuizmo::Manipulate(
            glm::value_ptr(viewMatrix),
            glm::value_ptr(projectionMatrix),
            gizmoOperation,
            ImGuizmo::LOCAL,
            glm::value_ptr(transformMatrix)
        );

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(transformMatrix, transform->Scale, transform->Rotation, transform->Position, skew, perspective);
            transform->Rotation = glm::conjugate(transform->Rotation);
        }
    }

    void EditorLayer::SelectEntityFromViewport(const ImVec2& mousePosition)
    {
        if (!m_WorldHierarchy)
            return;

        const float viewportAspect = viewportSize.y > 0.0f ? viewportSize.x / viewportSize.y : 1.0f;
        const glm::mat4 viewProjection = m_Camera.GetProjection(viewportAspect) * m_Camera.GetViewMatrix();

        Entity selectedEntity;
        float selectedDepth = std::numeric_limits<float>::max();

        m_World->Each<TransformComponent, SpriteRenderComponent>([&](const Entity& entity, const TransformComponent& transform, const SpriteRenderComponent&)
        {
            const glm::mat4 model = transform.GetWorldMatrix();
            const std::array<glm::vec3, 4> localCorners = {
                glm::vec3{-0.5f, -0.5f, 0.0f},
                glm::vec3{ 0.5f, -0.5f, 0.0f},
                glm::vec3{ 0.5f,  0.5f, 0.0f},
                glm::vec3{-0.5f,  0.5f, 0.0f}
            };

            std::array<ImVec2, 4> screenCorners{};
            float depth = 0.0f;
            for (std::size_t i = 0; i < localCorners.size(); ++i)
            {
                const glm::vec4 worldCorner = model * glm::vec4(localCorners[i], 1.0f);
                const glm::vec4 clip = viewProjection * worldCorner;
                if (clip.w <= 0.0f)
                    return;

                const glm::vec3 ndc = glm::vec3(clip) / clip.w;
                depth += ndc.z;
                screenCorners[i] = WorldToViewportScreen(glm::vec3(worldCorner), viewProjection, m_ViewportMin, viewportSize);
            }

            depth /= static_cast<float>(localCorners.size());
            if (IsPointInQuad(mousePosition, screenCorners) && depth < selectedDepth)
            {
                selectedDepth = depth;
                selectedEntity = entity;
            }
        });

        m_WorldHierarchy->SetSelectedEntity(selectedEntity);
    }

    void EditorLayer::DrawPanels()
    {
        ImGui::Begin("Panel");
        if (ImGui::Button("Add Entity"))
        {
            m_World->CreateEntity("Entity");
        }
        ImGui::End();

        m_WorldHierarchy->OnImGuiRender();
        m_ContentBrowserPanel.OnImGuiRender();
    }
} // Sunset
