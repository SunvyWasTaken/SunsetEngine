//
// Created by sunvy on 07/02/2026.
//

#ifndef SUNSETCRAFT_VERTEXARRAY_H
#define SUNSETCRAFT_VERTEXARRAY_H

namespace SunsetEngine
{
    class VertexBuffer;
    class IndiceBuffer;

    // VAO
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
        void AddIndexBuffer(const IndiceBuffer& indexBuffer);

        [[nodiscard]] uint32_t GetVAO() const;
        [[nodiscard]] uint32_t GetCount() const;
        [[nodiscard]] bool hasEbo() const;

    private:
        uint32_t m_Id;
        uint32_t count;
        bool bHasEbo;
    };
}

#endif //SUNSETCRAFT_VERTEXARRAY_H