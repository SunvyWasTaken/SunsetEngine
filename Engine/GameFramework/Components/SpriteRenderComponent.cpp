//
// Created by sunvy on 09/08/2026.
//

#include "SpriteRenderComponent.h"

#include "Render/Resources/Shader.h"
#include "Render/Resources/Mesh.h"
#include "Render/Resources/Material.h"

namespace
{
    constexpr std::array<glm::vec2, 6> points = {glm::vec2{-0.5f, -0.5f}, {-0.5f, 0.5f}, {0.5f, 0.5f}, {0.5f, 0.5f}, {0.5f, -0.5f}, {-0.5f, -0.5f}};
}

namespace Sunset
{
    SpriteRenderComponent::SpriteRenderComponent()
    {
        m_drawable.m_Mesh = Mesh::CreateMesh(points.data(), sizeof(glm::vec2), points.size(), {{VertexType::Float2}});
        m_drawable.m_Material->LoadShader(ENGINE_SHADERS_PATH "SpriteShader.vert", ENGINE_SHADERS_PATH "SpriteShader.frag");
    }

    template<typename Archive>
    void Serialize(Archive &ar, SpriteRenderComponent &component)
    {
        // ar(component);
    }
} // Sunset