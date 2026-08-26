//
// Created by sunvy on 26/08/2026.
//

#pragma once

#include "Render/Resources/Texture.h"

namespace Sunset
{
    class OpenGLTexture final : public Texture
    {
    public:
        OpenGLTexture(const TextureDescription& desc);

        ~OpenGLTexture() override;

        void Bind(std::uint32_t slot) const override;

        std::uint32_t GetWidth() const override;

        std::uint32_t GetHeight() const override;

        TextureFormat GetFormat() const override;
    private:
        std::uint32_t id;
        std::uint32_t width, height;
        TextureFormat format;
    };
} // Sunset