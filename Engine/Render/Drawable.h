//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_DRAWABLE_H
#define SUNSETCRAFT_DRAWABLE_H

namespace SunsetEngine
{
    class Material;
    class Textures;
    class Shader;
    class Mesh;

    enum class BlendFactor
    {
        Zero,
        One,
        SrcAlpha,
        OneMinusSrcAlpha,
        DstAlpha,
        OneMinusDstAlpha,
        SrcColor,
        OneMinusSrcColor,
        DstColor,
        OneMinusDstColor
    };

    enum class CullMode
    {
        None,
        Back,
        Front
    };

    enum class PrimitiveType
    {
        Points,
        Lines,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan
    };

    struct RenderState
    {
        bool depthTest = true;
        bool depthWrite = true;

        bool blending = false;
        BlendFactor src = BlendFactor::One;
        BlendFactor dest = BlendFactor::Zero;

        CullMode cullMode = CullMode::Front;
        bool wireframe = false;
        bool DrawInstance = false;
        bool HasIndice = true;
        PrimitiveType primitiveType = PrimitiveType::Triangles;
    };

    class Drawable final
    {
    public:
        Drawable();
        ~Drawable();

        std::shared_ptr<Mesh> m_Mesh;
        std::shared_ptr<Material> m_Material;
        glm::vec3 m_Position;
        RenderState m_RenderState;
    };
}


#endif //SUNSETCRAFT_DRAWABLE_H