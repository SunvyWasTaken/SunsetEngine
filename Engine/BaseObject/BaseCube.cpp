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
    std::shared_ptr<SunsetEngine::Drawable> DrawableCube = nullptr;

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
        // Front
        0, 1, 2,
        0, 2, 3,

        // Back
        5, 4, 7,
        5, 7, 6,

        // Left
        4, 0, 3,
        4, 3, 7,

        // Right
        1, 5, 6,
        1, 6, 2,

        // Top
        3, 2, 6,
        3, 6, 7,

        // Bottom
        4, 5, 1,
        4, 1, 0
    };
}

namespace SunsetEngine
{
    void DeleteBaseCube()
    {
        DrawableCube = nullptr;
    }

    void DrawCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec4 &color)
    {
        if (!DrawableCube)
        {
            SunsetEngine::BufferElement buffer{SunsetEngine::ShaderDataType::Float3, "position"};
            SunsetEngine::BufferElement Normal{SunsetEngine::ShaderDataType::Float3, "normal"};

            std::shared_ptr<SunsetEngine::Drawable> d = std::make_shared<SunsetEngine::Drawable>();
            d->m_Mesh = SunsetEngine::Mesh::CreateMesh(data.data(), sizeof(Vertex), data.size(), indices, {buffer, Normal});
            d->m_Material->LoadShader(ENGINE_SHADERS_PATH "Cube.vert", ENGINE_SHADERS_PATH "Cube.frag");
            DrawableCube = d;
        }

        SunsetEngine::Drawable d;
        d = *DrawableCube;
        d.m_Position = position;
        RenderCommande::Submit(d);
    }
}
