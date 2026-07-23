//
// Created by sunvy on 16/07/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"

namespace Sunset::OpenGLBuffer
{
    [[nodiscard]] std::uint32_t Create(BufferType type, const void* data, size_t size, BufferUsage usage);
    void Destroy(std::uint32_t buffer);
    void Bind(BufferType type, std::uint32_t buffer);
    void Update(BufferType type, std::uint32_t buffer, size_t offset, size_t size, const void* data);
}
