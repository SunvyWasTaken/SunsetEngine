//
// Created by sunvy on 19/12/2025.
//

#pragma once

#include "Render/Core/RenderHandle.h"

namespace Sunset
{
    class Shader;
    struct Image;

    class Texture
    {
    public:
        Texture();

        ~Texture();

        void Reset();

        void LoadImage(const std::string_view& fileName);

        void Use() const;

        TextureHandle GetRendererId() const;

    private:

        TextureHandle m_Id;
    };

    class Textures
    {
    public:
        Textures(const std::string_view& name, int width, int height);

        virtual ~Textures();

        void Use() const;

        const char* GetName() const;

        void AddImageAt(Image& image, const glm::ivec2& coord);

        size_t Nbr() const;

        TextureHandle GetRendererId() const;

        int m_Width, m_Height;

    private:
        std::string m_Name;
        TextureHandle m_Id;
        size_t m_Nbr;
    };
}
