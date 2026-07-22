//
// Created by sunvy on 07/02/2026.
//

#include "VertexArray.h"

#include "Buffers.h"
#include "Render/Core/RenderCommand.h"

namespace Sunset
{
    VertexArray::VertexArray()
       : m_Id(0)
       , count(0)
       , bHasEbo(false)
    {
        m_Id = RenderCommand::CreateVertexArray();
    }

    VertexArray::~VertexArray()
    {
        RenderCommand::DestroyVertexArray(m_Id);
    }

    void VertexArray::Bind() const
    {
        RenderCommand::BindVertexArray(m_Id);
    }

    void VertexArray::Unbind() const
    {
        RenderCommand::BindVertexArray(0);
    }

    void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        RenderCommand::ConfigureVertexArray(m_Id, vertexBuffer->GetRendererId(), vertexBuffer->GetLayout());
        count = vertexBuffer->GetSize();
    }

    void VertexArray::AddIndexBuffer(const IndiceBuffer& indexBuffer)
    {
        Bind();
        indexBuffer.Bind();
        count = indexBuffer.GetCount();
        bHasEbo = true;
        Unbind();
    }

    uint32_t VertexArray::GetRendererId() const
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
