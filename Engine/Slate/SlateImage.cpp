//
// Created by sunvy on 13/01/2026.
//

#include "SlateImage.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "Core/Application.h"
#include "Core/ApplicationSetting.h"
#include "Render/Buffers.h"
#include "Render/Drawable.h"
#include "Render/Image.h"
#include "Render/Material.h"
#include "Render/Mesh.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/VertexArray.h"

namespace
{
    std::weak_ptr<SunsetEngine::Shader> shader;

    struct vertice
    {
        glm::vec2 position;
        glm::vec2 uv;
    };

    std::array<vertice, 6> Square{
        vertice{{-1.f,  1.f}, {0, 0}}, // top-left
        vertice{{ 1.f,  1.f}, {1, 0}}, // top-right
        vertice{{ 1.f, -1.f}, {1, 1}}, // bottom-right

        vertice{{ 1.f, -1.f}, {1, 1}},
        vertice{{-1.f, -1.f}, {0, 1}},
        vertice{{-1.f,  1.f}, {0, 0}}
    };

}

namespace SunsetEngine
{
    SlateImage::SlateImage()
        : Slate()
        , m_Texture(nullptr)
    {
        m_Drawable = std::make_unique<Drawable>();
        if (shader.expired())
        {
            m_Drawable->m_Material->m_Shader = std::make_shared<Shader>("Engine/Shaders/SlateImage.vert", "Engine/Shaders/SlateImage.frag");
            shader = m_Drawable->m_Material->m_Shader;
        }
        else
            m_Drawable->m_Material->m_Shader = shader.lock();

        glm::ivec2 WindowSize = Application::GetSetting().WindowSize;
        m_Drawable->m_Material->Set("u_Proj", glm::ortho(0.0f, (float)WindowSize.x, (float)WindowSize.y, 0.0f, -1.0f, 1.0f));

        m_Drawable->m_RenderState.depthTest = false;
        m_Drawable->m_RenderState.depthWrite = true;
        // m_Drawable->m_RenderState.blending = true;
        // m_Drawable->m_RenderState.src = BlendFactor::SrcAlpha;
        // m_Drawable->m_RenderState.dest = BlendFactor::OneMinusSrcAlpha;
        m_Drawable->m_RenderState.cullMode = CullMode::None;
        // m_Drawable->m_RenderState.primitiveType = PrimitiveType::Triangles;
        m_Drawable->m_RenderState.HasIndice = false;
    }

    SlateImage::~SlateImage()
    {
        Clear();
    }

    void SlateImage::LoadImage(const std::string_view &path)
    {
        LOG("Engine", trace, "Slate image try to load {}", path);
        Image img{path};
        if (!img)
            return;

        std::vector<Image> images;
        images.emplace_back();
        images[0].width = img.width;
        images[0].height = img.height;
        images[0].nbrChannels = img.nbrChannels;
        images[0].SetData(img.m_Data);
        img.SetData(nullptr);

        m_Texture = std::make_shared<Textures>("U_Image", images, img.width, img.height);
        m_Drawable->m_Material->m_Textures.clear();
        m_Drawable->m_Material->m_Textures.emplace_back(m_Texture);

    }

    void SlateImage::Draw() const
    {
        const_cast<SlateImage*>(this)->Rebuild();

        if (!m_Texture || !m_Drawable->m_Mesh)
            return;

        Slate::Draw();
    }

    void SlateImage::Rebuild()
    {
        if (!bIsDirty)
            return;

        Clear();

        std::vector<vertice> vertices;
        ComputeVertex(vertices);

        auto vao = std::make_unique<VertexArray>();
        auto vbo = std::make_shared<VertexBuffer>(vertices.data(), vertices.size(), sizeof(vertice));
        vbo->SetLayout({
            {ShaderDataType::Float2, "pos"},
            {ShaderDataType::Float2, "uv"}
        });
        vao->AddVertexBuffer(vbo);

        m_Drawable->m_Mesh = std::make_shared<Mesh>(vao);
        m_Drawable->m_Mesh->m_VertexBuffer = vbo;

        bIsDirty = false;
    }

    void SlateImage::ComputeVertex(std::vector<vertice> &vertices)
    {
        vertices.clear();
        vertices.reserve(Square.size());

        for (const auto&[position, uv] : Square)
        {
            const glm::vec2 HalfSize{m_Size.x / 2.f, m_Size.y / 2.f};
            const glm::vec2 width{position.x * HalfSize.x, position.y * HalfSize.y};

            vertices.emplace_back(vertice{{width.x + m_Position.x, width.y + m_Position.y}, uv});
        }
    }

    void SlateImage::Clear()
    {
        m_Drawable->m_Mesh.reset();
    }
}
