//
// Created by sunvy on 13/01/2026.
//

#include "Image.h"

#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Sunset
{
    Image::Image(const std::string_view& path)
       : m_ImageName(path)
       , m_Data(nullptr)
       , width(0)
       , height(0)
       , nbrChannels(0)
    {
        if (!path.empty())
        {
            LoadImage(path);
        }
    }

    Image::~Image()
    {
        Clear();
    }

    void Image::LoadImage(const std::string_view& path)
    {
        stbi_set_flip_vertically_on_load(true);
        m_Data = stbi_load(path.data(), &width, &height, &nbrChannels, 0);
        if (m_Data == nullptr)
        {
            LOG("Engine", error, "Load Image failed : {}", path)
        }
    }

    void Image::Clear()
    {
        if (m_Data == nullptr)
            return;

        stbi_image_free(m_Data);
        m_Data = nullptr;
    }

    void Image::SetData(unsigned char* data)
    {
        m_Data = data;
    }

    unsigned int Image::Format() const
    {
        if (nbrChannels == 1)
            return 1;

        if (nbrChannels == 4)
            return 4;

        return 3;
    }

    Image::operator bool() const
    {
        return m_Data;
    }   
}
