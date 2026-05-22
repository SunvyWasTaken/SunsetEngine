//
// Created by sunvy on 11/02/2026.
//

#ifndef SUNSETCRAFT_MATERIAL_H
#define SUNSETCRAFT_MATERIAL_H

namespace SunsetEngine
{
    class Textures;
    class Shader;

    class Material
    {
        using UniformType = std::variant<float, int, glm::vec2, glm::vec3, glm::vec4, glm::mat4>;

        template<typename T, typename Variant>
        struct is_in_variant;

        template<typename T, typename... Ts>
        struct is_in_variant<T, std::variant<Ts...>>
            : std::disjunction<std::is_same<T, Ts>...> {};

    public:
        Material();
        ~Material();

        void Bind() const;

        void UniformBind() const;

        template <typename T>
        void Set(const std::string_view& name, const T& value)
        {
            static_assert(is_in_variant<T, UniformType>::value, "Type not allowed in Material::Set");
            m_Uniforms.insert_or_assign(name.data(), value);
        }

        std::shared_ptr<Shader> m_Shader;
        std::vector<std::shared_ptr<Textures>> m_Textures;

    private:
        std::unordered_map<std::string, UniformType> m_Uniforms;
    };
}

#endif //SUNSETCRAFT_MATERIAL_H