//
// Created by sunvy on 11/02/2026.
//

#include "Material.h"

#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"

namespace SunsetEngine
{
    Material::Material()
        : m_Shader(nullptr)
    {
    }

    Material::~Material()
    {
    }

    void Material::Bind() const
    {
        int index = 0;
        for (const auto& it : m_Textures)
        {
            glActiveTexture(GL_TEXTURE0 + index);
            it->Use();
            const GLint loc = glGetUniformLocation(m_Shader->GetId(), it->GetName());
            glUniform1i(loc, index);
            ++index;
        }
    }

    void Material::UniformBind() const
    {
        for (const auto& [name, value] : m_Uniforms)
        {
            std::visit(overloads
            {
                [&](const float val)
                {
                    m_Shader->SetFloat(name, val);
                },[&](const int val)
                {
                    m_Shader->SetInt(name, val);
                },[&](const glm::vec2& val)
                {
                    m_Shader->SetVec2(name, val);
                },[&](const glm::vec3& val)
                {
                    m_Shader->SetVec3(name, val);
                }, [&](const glm::vec4& val)
                {
                    m_Shader->SetVec4(name, val);
                }, [&](const glm::mat4& val)
                {
                    m_Shader->SetMat4(name, val);
                },
            }, value);
        }
    }
}
