//
// Created by sunvy on 26/08/2026.
//

#pragma once

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
    private:
        std::uint32_t id;
        std::size_t m_Size;
        BufferType m_Type;
    };
} // Sunset