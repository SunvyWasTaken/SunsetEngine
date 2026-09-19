//
// Created by sunvy on 07/07/2026.
//

#include "NativeScriptComponent.h"

#include "GameFramework/World/ScriptEntity.h"
#include "GameFramework/World/ScriptRegistry.h"
#include "Reflection/ReflectionType.h"
#include "SaveSystem/BinaryArchive.h"

namespace Sunset
{
    namespace
    {
        ReflectionField* FindField(ReflectionType& properties, const std::string& name, const ReflectionFieldType type)
        {
            const auto field = std::ranges::find_if(properties.Fields, [&name, type](const ReflectionField& candidate)
            {
                return candidate.Name == name && candidate.Type == type;
            });
            return field == properties.Fields.end() ? nullptr : &*field;
        }

        void CopyFieldValue(const ReflectionField& source, void* sourceInstance, const ReflectionField& target, void* targetInstance)
        {
            if (source.Type == ReflectionFieldType::Enum)
            {
                target.SetEnumValue(targetInstance, source.GetEnumValue(sourceInstance));
                return;
            }

            switch (source.Type)
            {
                case ReflectionFieldType::Float: *static_cast<float*>(target.GetPtr(targetInstance)) = *static_cast<float*>(source.GetPtr(sourceInstance)); break;
                case ReflectionFieldType::UInt8: *static_cast<std::uint8_t*>(target.GetPtr(targetInstance)) = *static_cast<std::uint8_t*>(source.GetPtr(sourceInstance)); break;
                case ReflectionFieldType::Int: *static_cast<int*>(target.GetPtr(targetInstance)) = *static_cast<int*>(source.GetPtr(sourceInstance)); break;
                case ReflectionFieldType::Bool: *static_cast<bool*>(target.GetPtr(targetInstance)) = *static_cast<bool*>(source.GetPtr(sourceInstance)); break;
                case ReflectionFieldType::Vec2: *static_cast<glm::vec2*>(target.GetPtr(targetInstance)) = *static_cast<glm::vec2*>(source.GetPtr(sourceInstance)); break;
                case ReflectionFieldType::Vec3: *static_cast<glm::vec3*>(target.GetPtr(targetInstance)) = *static_cast<glm::vec3*>(source.GetPtr(sourceInstance)); break;
                case ReflectionFieldType::Vec4: *static_cast<glm::vec4*>(target.GetPtr(targetInstance)) = *static_cast<glm::vec4*>(source.GetPtr(sourceInstance)); break;
                case ReflectionFieldType::String: *static_cast<std::string*>(target.GetPtr(targetInstance)) = *static_cast<std::string*>(source.GetPtr(sourceInstance)); break;
                case ReflectionFieldType::Enum: break;
            }
        }

        void SerializeField(BinaryOutputArchive& archive, const ReflectionField& field, void* instance)
        {
            std::string name = field.Name;
            ReflectionFieldType type = field.Type;
            archive(name);
            archive(type);

            if (type == ReflectionFieldType::Enum)
            {
                int value = field.GetEnumValue(instance);
                archive(value);
                return;
            }

            switch (type)
            {
                case ReflectionFieldType::Float: archive(*static_cast<float*>(field.GetPtr(instance))); break;
                case ReflectionFieldType::UInt8: archive(*static_cast<std::uint8_t*>(field.GetPtr(instance))); break;
                case ReflectionFieldType::Int: archive(*static_cast<int*>(field.GetPtr(instance))); break;
                case ReflectionFieldType::Bool: archive(*static_cast<bool*>(field.GetPtr(instance))); break;
                case ReflectionFieldType::Vec2: archive(*static_cast<glm::vec2*>(field.GetPtr(instance))); break;
                case ReflectionFieldType::Vec3: archive(*static_cast<glm::vec3*>(field.GetPtr(instance))); break;
                case ReflectionFieldType::Vec4: archive(*static_cast<glm::vec4*>(field.GetPtr(instance))); break;
                case ReflectionFieldType::String: archive(*static_cast<std::string*>(field.GetPtr(instance))); break;
                case ReflectionFieldType::Enum: break;
            }
        }

        void DeserializeField(BinaryInputArchive& archive, ScriptEntity* instance)
        {
            std::string name;
            ReflectionFieldType type{};
            archive(name);
            archive(type);

            ReflectionType properties = instance->Properties();
            ReflectionField* field = FindField(properties, name, type);

            if (type == ReflectionFieldType::Enum)
            {
                int value = 0;
                archive(value);
                if (field)
                    field->SetEnumValue(instance, value);
                return;
            }

            switch (type)
            {
                case ReflectionFieldType::Float: { float value{}; archive(value); if (field) *static_cast<float*>(field->GetPtr(instance)) = value; break; }
                case ReflectionFieldType::UInt8: { std::uint8_t value{}; archive(value); if (field) *static_cast<std::uint8_t*>(field->GetPtr(instance)) = value; break; }
                case ReflectionFieldType::Int: { int value{}; archive(value); if (field) *static_cast<int*>(field->GetPtr(instance)) = value; break; }
                case ReflectionFieldType::Bool: { bool value{}; archive(value); if (field) *static_cast<bool*>(field->GetPtr(instance)) = value; break; }
                case ReflectionFieldType::Vec2: { glm::vec2 value{}; archive(value); if (field) *static_cast<glm::vec2*>(field->GetPtr(instance)) = value; break; }
                case ReflectionFieldType::Vec3: { glm::vec3 value{}; archive(value); if (field) *static_cast<glm::vec3*>(field->GetPtr(instance)) = value; break; }
                case ReflectionFieldType::Vec4: { glm::vec4 value{}; archive(value); if (field) *static_cast<glm::vec4*>(field->GetPtr(instance)) = value; break; }
                case ReflectionFieldType::String: { std::string value; archive(value); if (field) *static_cast<std::string*>(field->GetPtr(instance)) = std::move(value); break; }
                case ReflectionFieldType::Enum: break;
            }
        }
    }

    NativeScriptComponent::NativeScriptComponent()
        : m_ScriptEntitys()
        , InstantiateScriptEntity()
    {
    }

    NativeScriptComponent::~NativeScriptComponent()
    {
        LOG("Engine", info, "NativeScriptComponent::~NativeScriptComponent()")
    }

    void NativeScriptComponent::Start(const Entity& entity)
    {
        InstantiateMissingScripts(entity);

        for (std::size_t index = 0; index < m_ScriptEntitys.size(); ++index)
        {
            if (m_ScriptStarted[index])
                continue;

            m_ScriptEntitys[index]->OnBeginPlay();
            m_ScriptStarted[index] = true;
        }
    }

    void NativeScriptComponent::Update(const float dt)
    {
        for (std::size_t index = 0; index < m_ScriptEntitys.size(); ++index)
        {
            if (m_ScriptStarted[index])
                m_ScriptEntitys[index]->OnUpdate(dt);
        }
    }

    void NativeScriptComponent::Stop()
    {
        for (std::size_t index = 0; index < m_ScriptEntitys.size(); ++index)
        {
            if (m_ScriptStarted[index])
                m_ScriptEntitys[index]->OnEndPlay();
        }

        m_ScriptEntitys.clear();
        m_ScriptStarted.clear();
    }

    void NativeScriptComponent::AddRegisteredScript(const std::string& name, std::function<ScriptEntity*()> instantiate)
    {
        if (std::ranges::find(RegisteredScriptNames, name) != RegisteredScriptNames.end())
            return;

        RegisteredScriptNames.emplace_back(name);
        m_RegisteredScriptIndices.emplace_back(InstantiateScriptEntity.size());
        AddScriptFactory(std::move(instantiate));
    }

    const std::vector<std::string>& NativeScriptComponent::GetRegisteredScriptNames() const
    {
        return RegisteredScriptNames;
    }

    void NativeScriptComponent::CopyConfigurationTo(NativeScriptComponent& target) const
    {
        target.InstantiateScriptEntity = InstantiateScriptEntity;
        target.RegisteredScriptNames = RegisteredScriptNames;
        target.m_RegisteredScriptIndices = m_RegisteredScriptIndices;
        target.InstantiateMissingScripts(target.GetOwner());

        const std::size_t count = std::min(m_ScriptEntitys.size(), target.m_ScriptEntitys.size());
        for (std::size_t index = 0; index < count; ++index)
        {
            ReflectionType sourceProperties = m_ScriptEntitys[index]->Properties();
            ReflectionType targetProperties = target.m_ScriptEntitys[index]->Properties();
            for (const ReflectionField& sourceField : sourceProperties.Fields)
            {
                if (ReflectionField* targetField = FindField(targetProperties, sourceField.Name, sourceField.Type))
                    CopyFieldValue(sourceField, m_ScriptEntitys[index].get(), *targetField, target.m_ScriptEntitys[index].get());
            }
        }
    }

    void NativeScriptComponent::AddScriptFactory(std::function<ScriptEntity*()> instantiate)
    {
        InstantiateScriptEntity.emplace_back(std::move(instantiate));
        InstantiateMissingScripts(GetOwner());
    }

    void NativeScriptComponent::InstantiateMissingScripts(const Entity& entity)
    {
        if (!entity)
            return;

        while (m_ScriptEntitys.size() < InstantiateScriptEntity.size())
        {
            std::unique_ptr<ScriptEntity> script{InstantiateScriptEntity[m_ScriptEntitys.size()]()};
            if (!script)
                break;

            script->m_Entity = entity;
            m_ScriptEntitys.emplace_back(std::move(script));
            m_ScriptStarted.emplace_back(false);
        }
    }

    void Serialize(BinaryInputArchive& archive, NativeScriptComponent& component)
    {
        component.Stop();
        component.RegisteredScriptNames.clear();
        component.m_RegisteredScriptIndices.clear();
        component.InstantiateScriptEntity.clear();

        std::uint64_t scriptCount = 0;
        archive(scriptCount);
        for (std::uint64_t index = 0; index < scriptCount; ++index)
        {
            std::string scriptName;
            archive(scriptName);
            const bool added = ScriptRegistry::AddScriptTo(component, scriptName);

            std::uint64_t fieldCount = 0;
            archive(fieldCount);
            const std::size_t scriptIndex = !added || component.m_RegisteredScriptIndices.empty()
                ? component.m_ScriptEntitys.size()
                : component.m_RegisteredScriptIndices.back();
            ScriptEntity* script = scriptIndex < component.m_ScriptEntitys.size() ? component.m_ScriptEntitys[scriptIndex].get() : nullptr;
            for (std::uint64_t field = 0; field < fieldCount; ++field)
            {
                if (script)
                    DeserializeField(archive, script);
                else
                {
                    // A missing registered script still has to consume its serialized fields.
                    std::string ignoredName;
                    ReflectionFieldType ignoredType{};
                    archive(ignoredName);
                    archive(ignoredType);
                    switch (ignoredType)
                    {
                        case ReflectionFieldType::Float: { float value{}; archive(value); break; }
                        case ReflectionFieldType::UInt8: { std::uint8_t value{}; archive(value); break; }
                        case ReflectionFieldType::Int: { int value{}; archive(value); break; }
                        case ReflectionFieldType::Bool: { bool value{}; archive(value); break; }
                        case ReflectionFieldType::Vec2: { glm::vec2 value{}; archive(value); break; }
                        case ReflectionFieldType::Vec3: { glm::vec3 value{}; archive(value); break; }
                        case ReflectionFieldType::Vec4: { glm::vec4 value{}; archive(value); break; }
                        case ReflectionFieldType::String: { std::string value; archive(value); break; }
                        case ReflectionFieldType::Enum: { int value{}; archive(value); break; }
                    }
                }
            }
        }
    }

    void Serialize(BinaryOutputArchive& archive, NativeScriptComponent& component)
    {
        std::uint64_t scriptCount = component.RegisteredScriptNames.size();
        archive(scriptCount);
        for (std::size_t index = 0; index < component.RegisteredScriptNames.size(); ++index)
        {
            std::string scriptName = component.RegisteredScriptNames[index];
            archive(scriptName);

            const std::size_t scriptIndex = component.m_RegisteredScriptIndices[index];
            ScriptEntity* script = scriptIndex < component.m_ScriptEntitys.size() ? component.m_ScriptEntitys[scriptIndex].get() : nullptr;
            std::uint64_t fieldCount = script ? script->Properties().Fields.size() : 0;
            archive(fieldCount);
            if (!script)
                continue;

            ReflectionType properties = script->Properties();
            for (const ReflectionField& field : properties.Fields)
                SerializeField(archive, field, script);
        }
    }
} // Sunset
