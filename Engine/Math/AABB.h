//
// Created by sunvy on 15/12/2025.
//

#ifndef SUNSETCRAFT_AABB_H
#define SUNSETCRAFT_AABB_H
#include <glm/vec3.hpp>

namespace SunsetEngine
{
    struct AABB
    {
        glm::vec3 min;
        glm::vec3 max;

        bool contains(const glm::vec3& point);

        bool intersects(const AABB& other);
    };
}

#endif //SUNSETCRAFT_AABB_H