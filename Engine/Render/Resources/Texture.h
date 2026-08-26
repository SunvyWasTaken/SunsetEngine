//
// Created by sunvy on 19/12/2025.
//

#pragma once

namespace Sunset
{
    class Shader;
    struct Image;

    enum class TextureFormat
    {
        Red,
        RGB,
        RGBA
    };

    struct TextureDescription
    {
        std::uint32_t width = 0, height = 0;
        TextureFormat format = TextureFormat::RGBA;
        const void* data = nullptr;
    };

    class Texture
    {
    public:
        static std::unique_ptr<Texture> Create(const TextureDescription& desc);
    public:
        virtual ~Texture() = default;

        virtual void Bind(std::uint32_t slot) const = 0;

        virtual std::uint32_t GetWidth() const = 0;
        virtual std::uint32_t GetHeight() const = 0;

        virtual TextureFormat GetFormat() const = 0;
    };
} // Sunset