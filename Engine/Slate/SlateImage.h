//
// Created by sunvy on 13/01/2026.
//

#ifndef SUNSETCRAFT_SLATEIMAGE_H
#define SUNSETCRAFT_SLATEIMAGE_H

#include "Slate.h"

namespace
{
    struct vertice;
}

namespace SunsetEngine
{
    class Textures;
    class Camera;
    class Shader;

    class SlateImage : public Slate
    {
    public:
        SlateImage();
        virtual ~SlateImage();
        void LoadImage(const std::string_view& path);
        void Draw() const override;
    private:
        void Rebuild();
        void ComputeVertex(std::vector<vertice>& vertices);
        void Clear();
    private:
        std::shared_ptr<Textures> m_Texture;
    };
}

#endif //SUNSETCRAFT_SLATEIMAGE_H