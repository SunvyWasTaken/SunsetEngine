//
// Created by sunvy on 07/02/2026.
//

#pragma once
#include "VertexLayout.h"

namespace Sunset
{
    class Buffer;

    class Mesh
    {
    public:
        static std::shared_ptr<Mesh> CreateMesh(const void* data, std::size_t typeSize, std::size_t size, const VertexLayout& layout);
    public:
        Mesh(const std::shared_ptr<Buffer>& vertexBuffer, const std::shared_ptr<Buffer>& indexBuffer, const VertexLayout& layout);
        virtual ~Mesh() = default;
        virtual void Bind() = 0;

        std::shared_ptr<Buffer> m_VertexBuffer;
        std::shared_ptr<Buffer> m_IndexBuffer;

        VertexLayout m_VertexLayout;
    };
} // Sunset