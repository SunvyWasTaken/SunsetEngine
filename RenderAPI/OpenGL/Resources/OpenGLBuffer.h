//
// Created by sunvy on 26/08/2026.
//

#pragma once

#include "Render/Core/RenderHandles.h"
#include "Render/Resources/Buffer.h"

namespace Sunset
{
    class OpenGLBuffer final : public Buffer
    {
    public:
        explicit OpenGLBuffer(const BufferType& type);

        ~OpenGLBuffer() override;

        void Bind() override;

        void SetData(const void *data, std::size_t typeSize, std::size_t size) override;

        [[nodiscard]] std::size_t Count() const override;

        [[nodiscard]] BufferHandle Get() const override;

    private:
        BufferHandle id;
        std::size_t m_Size;
        BufferType m_Type;
    };
} // Sunset