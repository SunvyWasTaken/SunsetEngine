//
// Created by sunvy on 19/12/2025.
//

#include "Texture.h"

#include "../Image.h"
#include "Render/Core/RenderCommand.h"
#include "Render/Core/RenderAPI.h"

namespace
{
    Sunset::TextureFormat TextureFormatFromChannelCount(const int channels)
    {
        if (channels == 1)
            return Sunset::TextureFormat::Red;
        if (channels == 4)
            return Sunset::TextureFormat::RGBA;

        return Sunset::TextureFormat::RGB;
    }
}

namespace Sunset
{
    /// Texture

    Texture::Texture()
    {
    }

    Texture::~Texture()
    {
        Reset();
    }

    void Texture::Reset()
    {
        RenderCommand::DestroyTexture(m_Id);
        m_Id = {};
    }

    void Texture::LoadImage(const std::string_view &fileName)
    {
        Image img{fileName};
        Reset();
        m_Id = RenderCommand::CreateTexture2D({
            .width = img.width,
            .height = img.height,
            .format = TextureFormatFromChannelCount(img.nbrChannels)
        }, img.m_Data);
    }

    void Texture::Use() const
    {
        RenderCommand::BindTexture(*this);
    }

    TextureHandle Texture::GetRendererId() const
    {
        return m_Id;
    }

    Textures::Textures(const std::string_view& name, const int width, const int height)
        : m_Width(width)
        , m_Height(height)
        , m_Name(name)
        , m_Id()
        , m_Nbr(0)
    {
        m_Id = RenderCommand::CreateTexture2D({
            .width = m_Width,
            .height = m_Height,
            .format = TextureFormat::RGB
        });
        LOG("Engine", trace, "Texture {} created at {}", m_Name, m_Id.id)
    }

    Textures::~Textures()
    {
        LOG("Engine", trace, "Texture {} destroy", m_Id.id)
        RenderCommand::DestroyTexture(m_Id);
    }

    void Textures::Use() const
    {
        RenderCommand::BindTexture(*this);
    }

    const char* Textures::GetName() const
    {
        return m_Name.c_str();
    }

    void Textures::AddImageAt(Image &image, const glm::ivec2& coord)
    {
        RenderCommand::UpdateTexture2D(m_Id, coord, {
            .width = image.width,
            .height = image.height,
            .format = TextureFormatFromChannelCount(image.nbrChannels)
        }, image.m_Data);
    }

    size_t Textures::Nbr() const
    {
        return m_Nbr;
    }

    TextureHandle Textures::GetRendererId() const
    {
        return m_Id;
    }

}
