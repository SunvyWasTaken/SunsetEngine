//
// Created by sunvy on 14/06/2026.
//

#pragma once

namespace Sunset
{
    struct Bone
    {
        std::string name;
        int parentIndex;
        glm::mat4 offset;
        glm::mat4 transform;
    };
} // Sunset