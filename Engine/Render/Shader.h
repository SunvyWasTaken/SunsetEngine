//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_SHADER_H
#define SUNSETCRAFT_SHADER_H

namespace SunsetEngine
{
    class Shader
    {
    public:
        Shader(const std::string_view& vertPath, const std::string_view& fragPath);
        ~Shader();

        std::uint32_t GetId() const;

        void Use() const;

        void SetFloat(const std::string_view& name, float value) const;
        void SetInt(const std::string_view& name, int value) const;
        void SetVec2(const std::string_view& name, const glm::vec2& vec) const;
        void SetVec3(const std::string_view& name, const glm::vec3& value) const;
        void SetVec4(const std::string_view& name, const glm::vec4& value) const;
        void SetMat4(const std::string_view& name, const glm::mat4 &value) const;

    private:
        std::uint32_t id;
    };
}

#endif //SUNSETCRAFT_SHADER_H