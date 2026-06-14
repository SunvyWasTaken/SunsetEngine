//
// Created by sunvy on 14/06/2026.
//

#include "UniformBuffer.h"

#include <glad/glad.h>

namespace Sunset
{
    template<typename T>
    UniformBuffer<T>::~UniformBuffer()
    {
        LOG("Engine", trace, "Uniform buffer {} delete", m_Id);
        glDeleteBuffers(1, &m_Id);
    }

    template<typename T>
    void UniformBuffer<T>::Init(size_t size)
    {
        m_Size = size;
        glGenBuffers(1, &m_Id);
        LOG("Engine", trace, "Uniform buffer {} Create", m_Id);
        glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(T) * size, nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    template<typename T>
    void UniformBuffer<T>::Update(const std::vector<T> &data) const
    {
        if (m_Size == 0)
        {
            LOG("Engine", warn, "U forgot to Init the uniform buffer first with a size");
            return;
        }
        glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(T) * m_Size, data.data());
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    template struct UniformBuffer<glm::mat4>;
} // Sunset