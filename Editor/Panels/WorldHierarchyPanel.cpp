//
// Created by sunvy on 07/07/2026.
//

#include "WorldHierarchyPanel.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.inl>

#include "GameFramework/Components/Component.h"
#include "GameFramework/Components/InputComponent.h"
#include "GameFramework/Components/NativeScriptComponent.h"
#include "GameFramework/Components/TransformComponent.h"
#include "GameFramework/World/Entity.h"
#include "GameFramework/World/ScriptEntity.h"

namespace
{
    const char* ToLabel(const Sunset::InputBindingType type)
    {
        switch (type)
        {
            case Sunset::InputBindingType::Keyboard: return "Keyboard";
            case Sunset::InputBindingType::Mouse: return "Mouse";
            case Sunset::InputBindingType::GamepadButton: return "Gamepad Button";
            case Sunset::InputBindingType::GamepadAxis: return "Gamepad Axis";
        }
        return "Unknown";
    }

    const char* ToLabel(const Sunset::Key key)
    {
        switch (key)
        {
            case Sunset::Key::Unknown: return "Unknown";
            case Sunset::Key::Space: return "Space";
            case Sunset::Key::Apostrophe: return "Apostrophe";
            case Sunset::Key::Comma: return "Comma";
            case Sunset::Key::Minus: return "Minus";
            case Sunset::Key::Period: return "Period";
            case Sunset::Key::Slash: return "Slash";
            case Sunset::Key::Num0: return "0";
            case Sunset::Key::Num1: return "1";
            case Sunset::Key::Num2: return "2";
            case Sunset::Key::Num3: return "3";
            case Sunset::Key::Num4: return "4";
            case Sunset::Key::Num5: return "5";
            case Sunset::Key::Num6: return "6";
            case Sunset::Key::Num7: return "7";
            case Sunset::Key::Num8: return "8";
            case Sunset::Key::Num9: return "9";
            case Sunset::Key::A: return "A";
            case Sunset::Key::B: return "B";
            case Sunset::Key::C: return "C";
            case Sunset::Key::D: return "D";
            case Sunset::Key::E: return "E";
            case Sunset::Key::F: return "F";
            case Sunset::Key::G: return "G";
            case Sunset::Key::H: return "H";
            case Sunset::Key::I: return "I";
            case Sunset::Key::J: return "J";
            case Sunset::Key::K: return "K";
            case Sunset::Key::L: return "L";
            case Sunset::Key::M: return "M";
            case Sunset::Key::N: return "N";
            case Sunset::Key::O: return "O";
            case Sunset::Key::P: return "P";
            case Sunset::Key::Q: return "Q";
            case Sunset::Key::R: return "R";
            case Sunset::Key::S: return "S";
            case Sunset::Key::T: return "T";
            case Sunset::Key::U: return "U";
            case Sunset::Key::V: return "V";
            case Sunset::Key::W: return "W";
            case Sunset::Key::X: return "X";
            case Sunset::Key::Y: return "Y";
            case Sunset::Key::Z: return "Z";
            case Sunset::Key::Escape: return "Escape";
            case Sunset::Key::Enter: return "Enter";
            case Sunset::Key::Tab: return "Tab";
            case Sunset::Key::Backspace: return "Backspace";
            case Sunset::Key::Insert: return "Insert";
            case Sunset::Key::Delete: return "Delete";
            case Sunset::Key::Left: return "Left";
            case Sunset::Key::Right: return "Right";
            case Sunset::Key::Up: return "Up";
            case Sunset::Key::Down: return "Down";
            case Sunset::Key::PageUp: return "Page Up";
            case Sunset::Key::PageDown: return "Page Down";
            case Sunset::Key::Home: return "Home";
            case Sunset::Key::End: return "End";
            case Sunset::Key::CapsLock: return "Caps Lock";
            case Sunset::Key::ScrollLock: return "Scroll Lock";
            case Sunset::Key::NumLock: return "Num Lock";
            case Sunset::Key::PrintScreen: return "Print Screen";
            case Sunset::Key::Pause: return "Pause";
            case Sunset::Key::F1: return "F1";
            case Sunset::Key::F2: return "F2";
            case Sunset::Key::F3: return "F3";
            case Sunset::Key::F4: return "F4";
            case Sunset::Key::F5: return "F5";
            case Sunset::Key::F6: return "F6";
            case Sunset::Key::F7: return "F7";
            case Sunset::Key::F8: return "F8";
            case Sunset::Key::F9: return "F9";
            case Sunset::Key::F10: return "F10";
            case Sunset::Key::F11: return "F11";
            case Sunset::Key::F12: return "F12";
            case Sunset::Key::LeftShift: return "Left Shift";
            case Sunset::Key::LeftControl: return "Left Control";
            case Sunset::Key::LeftAlt: return "Left Alt";
            case Sunset::Key::LeftSuper: return "Left Super";
            case Sunset::Key::RightShift: return "Right Shift";
            case Sunset::Key::RightControl: return "Right Control";
            case Sunset::Key::RightAlt: return "Right Alt";
            case Sunset::Key::RightSuper: return "Right Super";
            case Sunset::Key::Menu: return "Menu";
            case Sunset::Key::Count: break;
        }
        return "Unknown";
    }

    const char* ToLabel(const Sunset::MouseKey key)
    {
        switch (key)
        {
            case Sunset::MouseKey::Unknown: return "Unknown";
            case Sunset::MouseKey::Left: return "Left";
            case Sunset::MouseKey::Right: return "Right";
            case Sunset::MouseKey::Middle: return "Middle";
            case Sunset::MouseKey::Button_4: return "Button 4";
            case Sunset::MouseKey::Button_5: return "Button 5";
            case Sunset::MouseKey::Button_6: return "Button 6";
            case Sunset::MouseKey::Button_7: return "Button 7";
            case Sunset::MouseKey::Button_8: return "Button 8";
            case Sunset::MouseKey::Count: break;
        }
        return "Unknown";
    }

    const char* ToLabel(const Sunset::GamepadButton button)
    {
        switch (button)
        {
            case Sunset::GamepadButton::Unknown: return "Unknown";
            case Sunset::GamepadButton::South: return "South / A";
            case Sunset::GamepadButton::East: return "East / B";
            case Sunset::GamepadButton::West: return "West / X";
            case Sunset::GamepadButton::North: return "North / Y";
            case Sunset::GamepadButton::LeftBumper: return "Left Bumper";
            case Sunset::GamepadButton::RightBumper: return "Right Bumper";
            case Sunset::GamepadButton::Back: return "Back";
            case Sunset::GamepadButton::Start: return "Start";
            case Sunset::GamepadButton::Guide: return "Guide";
            case Sunset::GamepadButton::LeftThumb: return "Left Thumb";
            case Sunset::GamepadButton::RightThumb: return "Right Thumb";
            case Sunset::GamepadButton::DPadUp: return "DPad Up";
            case Sunset::GamepadButton::DPadRight: return "DPad Right";
            case Sunset::GamepadButton::DPadDown: return "DPad Down";
            case Sunset::GamepadButton::DPadLeft: return "DPad Left";
            case Sunset::GamepadButton::Count: break;
        }
        return "Unknown";
    }

    const char* ToLabel(const Sunset::GamepadAxis axis)
    {
        switch (axis)
        {
            case Sunset::GamepadAxis::Unknown: return "Unknown";
            case Sunset::GamepadAxis::LeftX: return "Left X";
            case Sunset::GamepadAxis::LeftY: return "Left Y";
            case Sunset::GamepadAxis::RightX: return "Right X";
            case Sunset::GamepadAxis::RightY: return "Right Y";
            case Sunset::GamepadAxis::LeftTrigger: return "Left Trigger";
            case Sunset::GamepadAxis::RightTrigger: return "Right Trigger";
            case Sunset::GamepadAxis::Count: break;
        }
        return "Unknown";
    }

    template <typename TEnum>
    bool DrawEnumCombo(const char* label, TEnum& value, std::uint16_t count, const char* (*labeler)(TEnum))
    {
        bool changed = false;
        int current = static_cast<int>(value);
        if (ImGui::BeginCombo(label, labeler(value)))
        {
            for (int i = 0; i < count; ++i)
            {
                const auto enumValue = static_cast<TEnum>(i);
                const bool selected = current == i;
                if (ImGui::Selectable(labeler(enumValue), selected))
                {
                    value = enumValue;
                    changed = true;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        return changed;
    }

    void DrawEditorObject(void* instance, Sunset::ReflectionType& properties)
    {
        for (auto& type : properties.Fields)
        {
            void* ptr = type.GetPtr(instance);
            switch (type.Type)
            {
                case Sunset::ReflectionFieldType::Float:
                {
                    float val = *(static_cast<float*>(ptr));
                    if (ImGui::InputFloat(type.Name.c_str(), &val, 0.01f))
                        *(static_cast<float*>(ptr)) = val;
                    break;
                }
                case Sunset::ReflectionFieldType::Bool:
                {
                    ImGui::Checkbox(type.Name.c_str(), static_cast<bool*>(ptr));
                    break;
                }
                case Sunset::ReflectionFieldType::UInt8:
                {
                    ImGui::InputInt(type.Name.c_str(), static_cast<int*>(ptr));
                    break;
                }
                case Sunset::ReflectionFieldType::Int:
                {
                    int val = *static_cast<std::uint8_t*>(ptr);
                    if (ImGui::InputInt(type.Name.c_str(), &val))
                        *(static_cast<std::uint8_t*>(ptr)) = val;
                    break;
                }
                case Sunset::ReflectionFieldType::String:
                {
                    char buffer[256] = {};
                    strcpy(buffer, static_cast<std::string*>(ptr)->c_str());
                    if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
                        static_cast<std::string*>(ptr)->assign(buffer);

                    break;
                }
                case Sunset::ReflectionFieldType::Vec2:
                {
                    ImGui::DragFloat2(type.Name.c_str(), static_cast<float*>(ptr));
                    break;
                }
                case Sunset::ReflectionFieldType::Vec3:
                {
                    ImGui::DragFloat3(type.Name.c_str(), static_cast<float*>(ptr));
                    break;
                }
                default:
                {

                }
            }
        }
    }
}

namespace Sunset
{
    WorldHierarchyPanel::WorldHierarchyPanel(const std::shared_ptr<World> &context)
    {
        SetContext(context);
    }

    void WorldHierarchyPanel::SetContext(const std::shared_ptr<World> &context)
    {
        m_Context = context;
    }

    void WorldHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Hierarchy");
        m_Context->m_Registry.view<entt::entity>().each([&](const entt::entity entity)
        {
            const Entity entt{m_Context.get(), entity};
            DrawNodeEntity(entt);
        });

        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectedEntity = {};
        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectedEntity)
        {
            DrawComponents(m_SelectedEntity);
        }
        ImGui::End();
    }

    void WorldHierarchyPanel::DrawNodeEntity(const Entity& entity)
    {
        if (const auto tag = entity.GetComponent<TagComponent>())
        {
            ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ?  ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
            bool opened = ImGui::TreeNodeEx((void*)(std::uint64_t)(std::uint32_t)entity, flags, tag->Tag.c_str());
            if (ImGui::IsItemClicked())
            {
                m_SelectedEntity = entity;
            }
            if (opened)
                ImGui::TreePop();
        }
    }

    void WorldHierarchyPanel::DrawComponents(Entity entity)
    {
        if (auto* tc = entity.GetComponent<TagComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(TagComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Name"))
            {
                ReflectionType properties = tc->Properties();
                DrawEditorObject(tc, properties);
                ImGui::TreePop();
            }
        }

        if (auto* tc = entity.GetComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& position = tc->Position;
                auto& rotation = tc->Rotation;
                auto& scale = tc->Scale;

                ImGui::DragFloat3("Position", glm::value_ptr(position));
                ImGui::DragFloat3("Rotation", glm::value_ptr(rotation));
                ImGui::DragFloat3("Scale", glm::value_ptr(scale));
                ImGui::TreePop();
            }
        }

        if (auto* tc = entity.GetComponent<NativeScriptComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(NativeScriptComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Script"))
            {
                for (auto& se : tc->m_ScriptEntitys)
                {
                    ReflectionType properties = se->Properties();
                    ImGui::Text(properties.Name.c_str());
                    DrawEditorObject(se.get(), properties);
                }
                ImGui::TreePop();
            }
        }

        if (auto* input = entity.GetComponent<InputComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(InputComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Input"))
            {
                auto& bindings = input->GetBindings();
                for (std::size_t i = 0; i < bindings.size(); ++i)
                {
                    auto& binding = bindings[i];
                    ImGui::PushID(static_cast<int>(i));
                    bool changed = false;

                    if (ImGui::TreeNodeEx("Binding", ImGuiTreeNodeFlags_DefaultOpen, "Binding %zu", i))
                    {
                        int action = static_cast<int>(binding.Action);
                        if (ImGui::InputInt("Action", &action))
                        {
                            binding.Action = static_cast<InputAction>(std::max(action, 0));
                            changed = true;
                        }

                        changed |= DrawEnumCombo("Type", binding.Type, 4, ToLabel);

                        if (binding.Type == InputBindingType::Keyboard)
                            changed |= DrawEnumCombo("Key", binding.KeyboardKey, static_cast<std::uint16_t>(Key::Count), ToLabel);
                        else if (binding.Type == InputBindingType::Mouse)
                            changed |= DrawEnumCombo("Mouse Button", binding.MouseButton, static_cast<std::uint16_t>(MouseKey::Count), ToLabel);
                        else if (binding.Type == InputBindingType::GamepadButton)
                        {
                            int gamepad = binding.Gamepad;
                            if (ImGui::InputInt("Gamepad", &gamepad))
                            {
                                binding.Gamepad = static_cast<std::uint8_t>(std::clamp(gamepad, 0, static_cast<int>(MaxGamepads - 1)));
                                changed = true;
                            }
                            changed |= DrawEnumCombo("Button", binding.PadButton, static_cast<std::uint16_t>(GamepadButton::Count), ToLabel);
                        }
                        else if (binding.Type == InputBindingType::GamepadAxis)
                        {
                            int gamepad = binding.Gamepad;
                            if (ImGui::InputInt("Gamepad", &gamepad))
                            {
                                binding.Gamepad = static_cast<std::uint8_t>(std::clamp(gamepad, 0, static_cast<int>(MaxGamepads - 1)));
                                changed = true;
                            }
                            changed |= DrawEnumCombo("Axis", binding.PadAxis, static_cast<std::uint16_t>(GamepadAxis::Count), ToLabel);
                            changed |= ImGui::DragFloat("Scale", &binding.Scale, 0.05f, -10.0f, 10.0f);
                        }

                        if (ImGui::Button("Remove Binding"))
                        {
                            input->RemoveBinding(i);
                            ImGui::TreePop();
                            ImGui::PopID();
                            break;
                        }

                        ImGui::TreePop();
                    }

                    if (changed)
                        input->RebuildMapping();

                    ImGui::PopID();
                }

                if (ImGui::Button("Add Keyboard Binding"))
                {
                    input->AddBinding({
                        .Type = InputBindingType::Keyboard,
                        .Action = 0,
                        .KeyboardKey = Key::Unknown
                    });
                }
                ImGui::SameLine();
                if (ImGui::Button("Add Mouse Binding"))
                {
                    input->AddBinding({
                        .Type = InputBindingType::Mouse,
                        .Action = 0,
                        .MouseButton = MouseKey::Left
                    });
                }
                if (ImGui::Button("Add Gamepad Button"))
                {
                    input->AddBinding({
                        .Type = InputBindingType::GamepadButton,
                        .Action = 0,
                        .PadButton = GamepadButton::South
                    });
                }
                ImGui::SameLine();
                if (ImGui::Button("Add Gamepad Axis"))
                {
                    input->AddBinding({
                        .Type = InputBindingType::GamepadAxis,
                        .Action = 0,
                        .PadAxis = GamepadAxis::LeftX,
                        .Scale = 1.0f
                    });
                }

                ImGui::TreePop();
            }
        }
    }
} // Sunset