//
// Created by sunvy on 07/02/2026.
//

#pragma once

namespace Sunset
{
    struct BufferElement;
    class IndiceBuffer;
    class VertexBuffer;
    class VertexArray;

    class Mesh
    {
    public:
        explicit Mesh(std::unique_ptr<VertexArray>& vao);
        virtual ~Mesh();

        static std::shared_ptr<Mesh> CreateMesh(const void* data, size_t typeSize, size_t size, const std::vector<uint32_t>& indices, const std::initializer_list<BufferElement>& layout);
        static std::shared_ptr<Mesh> CreateMesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndiceBuffer>& indiceBuffer);
        static std::shared_ptr<Mesh> CreateVertexOnly(const void* data, size_t typeSize, size_t size, const std::initializer_list<BufferElement>& layout);
        static std::shared_ptr<Mesh> CreateVertexOnly(const std::shared_ptr<VertexBuffer>& vertexBuffer);

        [[nodiscard]] uint32_t GetVAO() const;
        [[nodiscard]] uint32_t GetVertexCount() const;
    private:
        std::unique_ptr<VertexArray> m_VertexArray;

    public:
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndiceBuffer> m_IndiceBuffer;
    };
}
