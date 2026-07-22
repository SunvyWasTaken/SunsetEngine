//
// Created by sunvy on 14/06/2026.
//

#include "UniformBuffer.h"

#include "Render/Core/RenderAPI.h"
#include "Render/Core/RenderCommand.h"

namespace Sunset
{
    template<typename T>
    UniformBuffer<T>::~UniformBuffer()
    {
        LOG("Engine", trace, "Uniform buffer {} delete", m_Id);
        RenderCommand::DestroyBuffer(m_Id);
    }

    template<typename T>
    void UniformBuffer<T>::Init(size_t size)
    {
        m_Size = size;
        m_Id = RenderCommand::CreateBuffer(BufferType::Uniform, nullptr, sizeof(T) * size, BufferUsage::Dynamic);
        LOG("Engine", trace, "Uniform buffer {} Create", m_Id);
    }

    template<typename T>
    void UniformBuffer<T>::Update(const std::vector<T> &data) const
    {
        if (m_Size == 0)
        {
            LOG("Engine", warn, "U forgot to Init the uniform buffer first with a size");
            return;
        }
        RenderCommand::UpdateBuffer(BufferType::Uniform, m_Id, 0, sizeof(T) * m_Size, data.data());
    }

    template struct UniformBuffer<glm::mat4>;
} // Sunset
