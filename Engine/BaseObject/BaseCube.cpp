//
// Created by sunvy on 24/05/2026.
//

#include "BaseCube.h"

#include "Render/Drawable.h"
#include "Render/Material.h"
#include "Render/Mesh.h"
#include "Render/RenderCommande.h"
#include "Render/BufferObject/Buffers.h"

namespace
{
    std::shared_ptr<Sunset::Drawable> DrawableCube = nullptr;

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
    };

    const std::array<Vertex, 24> data{

        // Front (Z-)
        Vertex{{0,0,0}, {0,0,-1}},
        {{1,0,0}, {0,0,-1}},
        {{1,1,0}, {0,0,-1}},
        {{0,1,0}, {0,0,-1}},

        // Back (Z+)
        {{1,0,1}, {0,0,1}},
        {{0,0,1}, {0,0,1}},
        {{0,1,1}, {0,0,1}},
        {{1,1,1}, {0,0,1}},

        // Left (X-)
        {{0,0,1}, {-1,0,0}},
        {{0,0,0}, {-1,0,0}},
        {{0,1,0}, {-1,0,0}},
        {{0,1,1}, {-1,0,0}},

        // Right (X+)
        {{1,0,0}, {1,0,0}},
        {{1,0,1}, {1,0,0}},
        {{1,1,1}, {1,0,0}},
        {{1,1,0}, {1,0,0}},

        // Top (Y+)
        {{0,1,0}, {0,1,0}},
        {{1,1,0}, {0,1,0}},
        {{1,1,1}, {0,1,0}},
        {{0,1,1}, {0,1,0}},

        // Bottom (Y-)
        {{0,0,1}, {0,-1,0}},
        {{1,0,1}, {0,-1,0}},
        {{1,0,0}, {0,-1,0}},
        {{0,0,0}, {0,-1,0}},
    };

    const std::vector<uint32_t> indices{
        // Front (Z-)
        0, 1, 2,
        0, 2, 3,

        // Back (Z+)
        4, 5, 6,
        4, 6, 7,

        // Left (X-)
        8, 9, 10,
        8, 10, 11,

        // Right (X+)
        12, 13, 14,
        12, 14, 15,

        // Top (Y+)
        16, 17, 18,
        16, 18, 19,

        // Bottom (Y-)
        20, 21, 22,
        20, 22, 23
    };
}

namespace Sunset
{
    void DeleteBaseCube()
    {
        DrawableCube = nullptr;
    }

    void DrawCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec4 &color)
    {
        if (!DrawableCube)
        {
            Sunset::BufferElement buffer{Sunset::ShaderDataType::Float3, "position"};
            Sunset::BufferElement Normal{Sunset::ShaderDataType::Float3, "normal"};

            std::shared_ptr<Sunset::Drawable> d = std::make_shared<Sunset::Drawable>();
            d->m_Mesh = Sunset::Mesh::CreateMesh(data.data(), sizeof(Vertex), data.size(), indices, {buffer, Normal});
            d->m_Material->LoadShader(ENGINE_SHADERS_PATH "Cube.vert", ENGINE_SHADERS_PATH "Cube.frag");
            DrawableCube = d;
        }

        Sunset::Drawable d;
        d = *DrawableCube;
        d.m_Position = position;
        RenderCommande::Submit(d);
    }

    void DrawCube(const glm::mat4 &transform, const glm::vec4 &color, bool wireframe)
    {
        if (!DrawableCube)
        {
            Sunset::BufferElement buffer{Sunset::ShaderDataType::Float3, "position"};
            Sunset::BufferElement Normal{Sunset::ShaderDataType::Float3, "normal"};

            std::shared_ptr<Sunset::Drawable> d = std::make_shared<Sunset::Drawable>();
            d->m_Mesh = Sunset::Mesh::CreateMesh(data.data(), sizeof(Vertex), data.size(), indices, {buffer, Normal});
            d->m_Material->LoadShader(ENGINE_SHADERS_PATH "Cube.vert", ENGINE_SHADERS_PATH "Cube.frag");
            DrawableCube = d;
        }

        Sunset::Drawable d;
        d = *DrawableCube;
        RenderCommande::Submit(d, transform);
    }
}
