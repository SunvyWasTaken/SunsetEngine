//
// Created by sunvy on 07/02/2026.
//

#include "VertexArray.h"

#include "Buffers.h"

#include <glad/glad.h>

namespace SunsetEngine
{
    VertexArray::VertexArray()
       : m_Id(0)
       , count(0)
    {
        glGenVertexArrays(1, &m_Id);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_Id);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_Id);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        Bind();
        vertexBuffer->Bind();

        const auto& layout = vertexBuffer->GetLayout();
        uint32_t index = 0;

        for (auto& element : layout)
        {
            glEnableVertexAttribArray(index);
            if (element.IsInt())
                glVertexAttribIPointer(index, element.Count(), element.Type(), layout.GetStride(), (const void*)element.offset);
            else
                glVertexAttribPointer(index, element.Count(), element.Type(), element.normalized, layout.GetStride(), (const void*)element.offset);
            if (element.divisor > 0)
                glVertexAttribDivisor(index, element.divisor);

            index++;
        }
        count = vertexBuffer->GetSize();
        Unbind();
    }

    void VertexArray::AddIndexBuffer(const IndiceBuffer& indexBuffer)
    {
        Bind();
        indexBuffer.Bind();
        count = indexBuffer.GetCount();
        Unbind();
    }

    uint32_t VertexArray::GetVAO() const
    {
        return m_Id;
    }

    uint32_t VertexArray::GetCount() const
    {
        return count;
    }

    bool VertexArray::hasEbo() const
    {
        return bHasEbo;
    }
}
