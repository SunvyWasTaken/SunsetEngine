//
// Created by sunvy on 07/02/2026.
//

#include "Mesh.h"

#include "VertexArray.h"

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

    uint32_t Mesh::GetVAO() const
    {
        return m_VertexArray->GetVAO();
    }

    uint32_t Mesh::GetVertexCount() const
    {
        return m_VertexArray->GetCount();
    }
}
