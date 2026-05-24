//
// Created by sunvy on 24/05/2026.
//

#include "BaseCube.h"

#include "Render/Drawable.h"
#include "Render/Material.h"
#include "Render/Mesh.h"
#include "Render/RenderCommande.h"
#include "Render/Shader.h"
#include "Render/BufferObject/Buffers.h"

namespace
{
    std::shared_ptr<SunsetEngine::Drawable> DrawableCube = nullptr;

    const std::array<glm::vec3, 36> data = std::array<glm::vec3, 36>{
        // Front
        glm::vec3(0,0,0), glm::vec3(1,0,0), glm::vec3(1,1,0),
        glm::vec3(0,0,0), glm::vec3(1,1,0), glm::vec3(0,1,0),

        // Back
        glm::vec3(1,0,1), glm::vec3(0,0,1), glm::vec3(0,1,1),
        glm::vec3(1,0,1), glm::vec3(0,1,1), glm::vec3(1,1,1),

        // Left
        glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0),
        glm::vec3(0,0,1), glm::vec3(0,1,0), glm::vec3(0,1,1),

        // Right
        glm::vec3(1,0,0), glm::vec3(1,0,1), glm::vec3(1,1,1),
        glm::vec3(1,0,0), glm::vec3(1,1,1), glm::vec3(1,1,0),

        // Top
        glm::vec3(0,1,0), glm::vec3(1,1,0), glm::vec3(1,1,1),
        glm::vec3(0,1,0), glm::vec3(1,1,1), glm::vec3(0,1,1),

        // Bottom
        glm::vec3(0,0,1), glm::vec3(1,0,1), glm::vec3(1,0,0),
        glm::vec3(0,0,1), glm::vec3(1,0,0), glm::vec3(0,0,0)
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

            std::shared_ptr<SunsetEngine::Drawable> d = std::make_shared<SunsetEngine::Drawable>();
            d->m_Mesh = SunsetEngine::Mesh::CreateVertexOnly(data.data(), sizeof(glm::vec3), data.size(), {buffer});
            d->m_Material->LoadShader(ENGINE_SHADERS_PATH "Cube.vert", ENGINE_SHADERS_PATH "Cube.frag");
            d->m_RenderState.HasIndice = false;
            DrawableCube = d;
        }

        std::shared_ptr<SunsetEngine::Drawable> d;
        d = DrawableCube;
        d->m_Position = position;
        RenderCommande::Submit(*d);
    }
}
