//
// Created by sunvy on 14/07/2026.
//

#pragma once

namespace Sunset
{
    enum class ReflectionFieldType
    {
        Float,
        Int,
        Bool,
        Vec2,
        Vec3,
        String
    };

    template <typename T>
    constexpr ReflectionFieldType GetReflectionFieldType()
    {
        static_assert(always_false_v<T>, "ReflectionFieldType<T> is not implemented");
        return ReflectionFieldType::Float;
    }

    template <>
    constexpr ReflectionFieldType GetReflectionFieldType<float>()
    {
        return ReflectionFieldType::Float;
    }

    template <>
    constexpr ReflectionFieldType GetReflectionFieldType<int>()
    {
        return ReflectionFieldType::Int;
    }

    template <>
    constexpr ReflectionFieldType GetReflectionFieldType<bool>()
    {
        return ReflectionFieldType::Bool;
    }

    template <>
    constexpr ReflectionFieldType GetReflectionFieldType<glm::vec2>()
    {
        return ReflectionFieldType::Vec2;
    }

    template <>
    constexpr ReflectionFieldType GetReflectionFieldType<glm::vec3>()
    {
        return ReflectionFieldType::Vec3;
    }

    template <>
    constexpr ReflectionFieldType GetReflectionFieldType<std::string>()
    {
        return ReflectionFieldType::String;
    }

    struct ReflectionField
    {
        std::string Name;
        ReflectionFieldType Type{};
        std::function<void*(void*)> GetPtr;
    };

    struct ReflectionType
    {
        std::vector<ReflectionField> Fields;

        template <typename Class, typename F>
        void Field(const std::string& name, F Class::* member)
        {
            ReflectionField reflectionField;
            reflectionField.Name = name;
            reflectionField.Type = GetReflectionFieldType<F>();
            reflectionField.GetPtr = [member](void* instance) -> void*
            {
                return &(static_cast<Class*>(instance)->*member);
            };

            Fields.push_back(std::move(reflectionField));
        }
    };
}
