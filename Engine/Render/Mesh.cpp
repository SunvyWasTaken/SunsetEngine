//
// Created by sunvy on 07/02/2026.
//

#include "Mesh.h"

#include "BufferObject/Buffers.h"
#include "BufferObject/VertexArray.h"

namespace SunsetEngine
{
    Mesh::Mesh(std::unique_ptr<VertexArray>& vao)
        : m_VertexArray(std::move(vao))
        , m_VertexBuffer(nullptr)
        , m_IndiceBuffer(nullptr)
    {
    }

    Mesh::~Mesh()
    {
    }

    std::shared_ptr<Mesh> Mesh::CreateVertexOnly(const void *data, size_t typeSize, size_t size,
        const std::initializer_list<BufferElement> &layout)
    {
        auto vertexBuffer = std::make_shared<VertexBuffer>(data, typeSize, size);
        vertexBuffer->SetLayout(layout);
        return CreateVertexOnly(vertexBuffer);
    }

    std::shared_ptr<Mesh> Mesh::CreateVertexOnly(const std::shared_ptr<VertexBuffer> &vertexBuffer)
    {
        auto vao = std::make_unique<VertexArray>();
        vao->AddVertexBuffer(vertexBuffer);

        auto mesh = std::make_shared<Mesh>(vao);
        mesh->m_VertexBuffer = vertexBuffer;
        return mesh;
    }

    uint32_t Mesh::GetVAO() const
    {
        return m_VertexArray->GetVAO();
    }

    uint32_t Mesh::GetVertexCount() const
    {
        return m_VertexArray->GetCount();
    }
}
