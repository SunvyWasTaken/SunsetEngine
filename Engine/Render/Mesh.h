//
// Created by sunvy on 07/02/2026.
//

#ifndef SUNSETCRAFT_MESH_H
#define SUNSETCRAFT_MESH_H

namespace SunsetEngine
{
    class IndiceBuffer;
    class VertexBuffer;
    class VertexArray;

    class Mesh final
    {
    public:
        explicit Mesh(std::unique_ptr<VertexArray>& vao);
        ~Mesh();
        [[nodiscard]] uint32_t GetVAO() const;
        [[nodiscard]] uint32_t GetVertexCount() const;
    private:
        std::unique_ptr<VertexArray> m_VertexArray;

    public:
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndiceBuffer> m_IndiceBuffer;
    };
}

#endif //SUNSETCRAFT_MESH_H