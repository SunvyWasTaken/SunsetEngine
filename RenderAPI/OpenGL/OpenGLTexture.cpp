//
// Created by sunvy on 16/07/2026.
//

#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace
{
    GLenum ToGLTextureFormat(const Sunset::TextureFormat format)
    {
        switch (format)
        {
            case Sunset::TextureFormat::Red:  return GL_RED;
            case Sunset::TextureFormat::RGB:  return GL_RGB;
            case Sunset::TextureFormat::RGBA: return GL_RGBA;
        }

        return GL_RGB;
    }
}

namespace Sunset::OpenGLTexture
{
    std::uint32_t Create2D(const TextureSpecification& specification, const void* data)
    {
        std::uint32_t texture = 0;
        const GLenum format = ToGLTextureFormat(specification.format);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, specification.width, specification.height, 0, format, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    }

    void Destroy(const std::uint32_t texture)
    {
        glDeleteTextures(1, &texture);
    }

    void Bind(const std::uint32_t texture, const std::uint32_t slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void Update2D(const std::uint32_t texture, const glm::ivec2& coord, const TextureSpecification& specification, const void* data)
    {
        const GLenum format = ToGLTextureFormat(specification.format);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, coord.x, coord.y, specification.width, specification.height, format, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
