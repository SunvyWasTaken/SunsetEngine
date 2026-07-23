//
// Created by sunvy on 07/02/2026.
//

#ifndef SUNSETCRAFT_VERTEXARRAY_H
#define SUNSETCRAFT_VERTEXARRAY_H

#include "Render/Core/RenderHandle.h"

namespace Sunset
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

        [[nodiscard]] VertexArrayHandle GetRendererId() const;
        [[nodiscard]] uint32_t GetCount() const;
        [[nodiscard]] bool hasEbo() const;

    private:
        VertexArrayHandle m_Id;
        uint32_t count;
        bool bHasEbo;
    };
}

#endif //SUNSETCRAFT_VERTEXARRAY_H
