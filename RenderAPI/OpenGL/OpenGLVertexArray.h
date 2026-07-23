//
// Created by Codex on 23/07/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    class BufferLayout;
}

namespace Sunset::OpenGLVertexArray
{
    [[nodiscard]] std::uint32_t Create();
    void Destroy(std::uint32_t vertexArray);
    void Bind(std::uint32_t vertexArray);
    void Configure(std::uint32_t vertexArray, std::uint32_t vertexBuffer, const BufferLayout& layout);
}
