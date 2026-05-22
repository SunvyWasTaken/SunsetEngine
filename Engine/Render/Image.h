//
// Created by sunvy on 13/01/2026.
//

#ifndef SUNSETCRAFT_IMAGE_H
#define SUNSETCRAFT_IMAGE_H

namespace SunsetEngine
{
    class Image
    {
    public:
        explicit Image(const std::string_view& path = std::string_view{});
        virtual ~Image();
        void LoadImage(const std::string_view& path);
        void Clear();
        void SetData(unsigned char* data);
        unsigned int Format() const;
        explicit operator bool() const;

        std::string m_ImageName;
        unsigned char* m_Data;
        int width, height, nbrChannels;
    };
}

#endif //SUNSETCRAFT_IMAGE_H