//
// Created by sunvy on 15/12/2025.
//

#include "AABB.h"

namespace SunsetEngine
{
    bool AABB::contains(const glm::vec3& point)
    {
        return (point.x >= min.x && point.x <= max.x) &&
       (point.y >= min.y && point.y <= max.y) &&
       (point.z >= min.z && point.z <= max.z);
    }

    bool AABB::intersects(const AABB& other)
    {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
       (min.y <= other.max.y && max.y >= other.min.y) &&
       (min.z <= other.max.z && max.z >= other.min.z);
    }
}
