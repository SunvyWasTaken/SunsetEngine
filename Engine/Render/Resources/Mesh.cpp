//
// Created by sunvy on 07/02/2026.
//

#include "Mesh.h"

#include "Buffer.h"
#include "Core/Application.h"
#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    std::shared_ptr<Mesh> Mesh::CreateMesh(const void *data, std::size_t typeSize, std::size_t size,
        const VertexLayout &layout)
    {
        auto vertexBuffer = Buffer::Create(BufferType::Vertex);
        vertexBuffer->SetData(data, typeSize, size);
        std::shared_ptr<Buffer> indexBuffer = nullptr;
        return Application::GetAPI()->CreateMesh(vertexBuffer, indexBuffer, layout);
    }

    Mesh::Mesh(const std::shared_ptr<Buffer> &vertexBuffer, const std::shared_ptr<Buffer> &indexBuffer,
               const VertexLayout &layout)
            : m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_VertexLayout(layout)
    {
    }
} // Sunset