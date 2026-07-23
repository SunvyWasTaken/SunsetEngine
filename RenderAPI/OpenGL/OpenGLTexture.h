//
// Created by sunvy on 16/07/2026.
//

#pragma once

#include "Render/Core/RenderAPI.h"

namespace Sunset::OpenGLTexture
{
    [[nodiscard]] std::uint32_t Create2D(const TextureSpecification& specification, const void* data);
    void Destroy(std::uint32_t texture);
    void Bind(std::uint32_t texture, std::uint32_t slot);
    void Update2D(std::uint32_t texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data);
}
