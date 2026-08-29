//
// Created by sunvy on 29/08/2026.
//

#pragma once

#include <glm/fwd.hpp>

#include "RenderHandles.h"

namespace Sunset
{
    struct DrawCmd final
    {
        DrawCmd(const MeshHandle& meshHandle, const MaterialHandle& materialHandle, const glm::mat4& modelMatrix)
            : mesh(meshHandle), material(materialHandle), transform(modelMatrix)
        {}

        MeshHandle mesh;
        MaterialHandle material;
        glm::mat4 transform;
    };
}
