//
// Created by sunvy on 24/05/2026.
//

#pragma once

namespace Sunset
{
    void DeleteBaseCube();

    void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color);

    void DrawCube(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), bool wireframe = false);
}
