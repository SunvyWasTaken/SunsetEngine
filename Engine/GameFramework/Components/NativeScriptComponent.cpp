//
// Created by sunvy on 07/07/2026.
//

#include "NativeScriptComponent.h"

#include "GameFramework/World/ScriptEntity.h"
#include "GameFramework/World/ScriptRegistry.h"
#include "SaveSystem/BinaryArchive.h"

namespace Sunset
{
    NativeScriptComponent::NativeScriptComponent()
        : m_ScriptEntitys()
        , InstantiateScriptEntity()
        // , DestroyScriptEntity(nullptr)
    {
    }

    NativeScriptComponent::~NativeScriptComponent()
    {
        LOG("Engine", info, "NativeScriptComponent::~NativeScriptComponent()")
    }

    void NativeScriptComponent::Start(const Entity& entity)
    {
        if (!m_ScriptEntitys.empty())
            return;

        for (auto& i : InstantiateScriptEntity)
        {
            m_ScriptEntitys.emplace_back(i());
            m_ScriptEntitys.back()->m_Entity = entity;
            m_ScriptEntitys.back()->OnBeginPlay();
        }
    }

    void NativeScriptComponent::Stop()
    {
        for (const auto& scriptEntity : m_ScriptEntitys)
            scriptEntity->OnEndPlay();

        m_ScriptEntitys.clear();
    }

    void NativeScriptComponent::AddRegisteredScript(const std::string& name, std::function<ScriptEntity*()> instantiate)
    {
        if (std::ranges::find(RegisteredScriptNames, name) != RegisteredScriptNames.end())
            return;

        RegisteredScriptNames.emplace_back(name);
        InstantiateScriptEntity.emplace_back(std::move(instantiate));
    }

    const std::vector<std::string>& NativeScriptComponent::GetRegisteredScriptNames() const
    {
        return RegisteredScriptNames;
    }

    void Serialize(BinaryInputArchive& archive, NativeScriptComponent& component)
    {
        std::vector<std::string> scriptNames;
        archive(scriptNames);

        component.Stop();
        component.RegisteredScriptNames.clear();
        component.InstantiateScriptEntity.clear();

        for (const auto& scriptName : scriptNames)
            ScriptRegistry::AddScriptTo(component, scriptName);
    }

    void Serialize(BinaryOutputArchive& archive, NativeScriptComponent& component)
    {
        std::vector<std::string> scriptNames = component.GetRegisteredScriptNames();
        archive(scriptNames);
    }
} // Sunset
