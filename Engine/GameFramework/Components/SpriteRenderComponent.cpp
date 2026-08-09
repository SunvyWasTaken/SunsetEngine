//
// Created by sunvy on 09/08/2026.
//

#include "SpriteRenderComponent.h"

#include "Render/BufferObject/Buffers.h"
#include "Render/Core/Shader.h"
#include "Render/Meshes/Mesh.h"
#include "Render/Resources/Material.h"

namespace
{
    constexpr std::array<glm::vec2, 6> points = {glm::vec2{0.f, 0.f}, {0.f, 1.f}, {1.f, 1.f}, {1.f, 1.f}, {1.f, 0.f}, {0.f, 0.f}};
}

namespace Sunset
{
    SpriteRenderComponent::SpriteRenderComponent()
    {
        m_drawable.m_Mesh = Mesh::CreateVertexOnly(points.data(), sizeof(glm::vec2), points.size(), {BufferElement{ShaderDataType::Float2, "pos"}});
        m_drawable.m_Material->LoadShader(ENGINE_SHADERS_PATH "SpriteShader.vert", ENGINE_SHADERS_PATH "SpriteShader.frag");
    }
} // Sunset