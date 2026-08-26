//
// Created by sunvy on 26/08/2026.
//

#include "OpenGLTexture.h"

#include <glad/glad.h>

namespace
{
    GLenum ToGLTextureFormat(Sunset::TextureFormat format)
    {
        switch (format)
        {
            case Sunset::TextureFormat::Red: return GL_RED;
            case Sunset::TextureFormat::RGB: return GL_RGB;
            case Sunset::TextureFormat::RGBA: return GL_RGBA;
        }
        return GL_RGB;
    }
}

namespace Sunset
{
    OpenGLTexture::OpenGLTexture(const TextureDescription &desc)
        : id(0)
        , width(desc.width)
        , height(desc.height)
        , format(desc.format)
    {
        if (!desc.data)
        {
            LOG("OpenGL", error, "Texture desc")
            DEBUG_BREAK();
            return;
        }

        const GLenum format = ToGLTextureFormat(desc.format);

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, desc.data);
        glBindTexture(GL_TEXTURE_2D, 0);

        LOG("OpenGL", trace, "Texture {} created", id)
    }

    OpenGLTexture::~OpenGLTexture()
    {
        LOG("OpenGL", trace, "Texture {} destroyed", id)
        glDeleteTextures(1, &id);
    }

    void OpenGLTexture::Bind(std::uint32_t slot) const
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    std::uint32_t OpenGLTexture::GetWidth() const
    {
        return width;
    }

    std::uint32_t OpenGLTexture::GetHeight() const
    {
        return height;
    }

    TextureFormat OpenGLTexture::GetFormat() const
    {
        return format;
    }
} // Sunset