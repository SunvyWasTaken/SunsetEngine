//
// Created by sunvy on 14/07/2026.
//

#pragma once

namespace Sunset
{
    enum class ReflectionFieldType
    {
        Float,
        UInt8,
        Int,
        Bool,
        Vec2,
        Vec3,
        String,
        Enum
    };

    template <typename T>
    constexpr ReflectionFieldType GetReflectionFieldType()
    {
        static_assert(always_false_v<T>, "ReflectionFieldType<T> is not implemented");
        return ReflectionFieldType::Float;
    }

    template <>
    constexpr ReflectionFieldType GetReflectionFieldType<std::uint8_t>()
    {
        return ReflectionFieldType::UInt8;
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
        std::vector<std::pair<int, std::string>> EnumValues;
        std::function<int(void*)> GetEnumValue;
        std::function<void(void*, int)> SetEnumValue;
    };

    struct ReflectionType
    {
        std::string Name;
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

        template <typename F>
        void Field(const std::string& name, ReflectionFieldType type, std::function<F*(void*)> getter)
        {
            ReflectionField reflectionField;
            reflectionField.Name = name;
            reflectionField.Type = type;
            reflectionField.GetPtr = [getter = std::move(getter)](void* instance) -> void*
            {
                return getter(instance);
            };

            Fields.push_back(std::move(reflectionField));
        }

        template <typename Class, typename Enum>
        void EnumField(const std::string& name, Enum Class::* member, std::vector<std::pair<Enum, std::string>> values)
        {
            ReflectionField reflectionField;
            reflectionField.Name = name;
            reflectionField.Type = ReflectionFieldType::Enum;
            reflectionField.GetEnumValue = [member](void* instance) -> int
            {
                return static_cast<int>(static_cast<Class*>(instance)->*member);
            };
            reflectionField.SetEnumValue = [member](void* instance, const int value)
            {
                static_cast<Class*>(instance)->*member = static_cast<Enum>(value);
            };

            for (auto& [value, label] : values)
                reflectionField.EnumValues.emplace_back(static_cast<int>(value), std::move(label));

            Fields.push_back(std::move(reflectionField));
        }

        template <typename Class, typename Enum>
        void EnumField(const std::string& name, std::function<Enum*(void*)> getter, std::vector<std::pair<Enum, std::string>> values)
        {
            ReflectionField reflectionField;
            reflectionField.Name = name;
            reflectionField.Type = ReflectionFieldType::Enum;
            reflectionField.GetEnumValue = [getter](void* instance) -> int
            {
                return static_cast<int>(*getter(instance));
            };
            reflectionField.SetEnumValue = [getter](void* instance, const int value)
            {
                *getter(instance) = static_cast<Enum>(value);
            };

            for (auto& [value, label] : values)
                reflectionField.EnumValues.emplace_back(static_cast<int>(value), std::move(label));

            Fields.push_back(std::move(reflectionField));
        }
    };
}
