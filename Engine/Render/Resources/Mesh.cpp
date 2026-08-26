//
// Created by sunvy on 07/02/2026.
//

#include "Mesh.h"

namespace Sunset
{
    Mesh::Mesh(const std::shared_ptr<Buffer> &vertexBuffer, const std::shared_ptr<Buffer> &indexBuffer,
        const VertexLayout &layout)
            : m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_VertexLayout(layout)
    {
    }
} // Sunset