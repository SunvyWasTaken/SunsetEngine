//
// Created by sunvy on 14/06/2026.
//

#include "Skeleton.h"

namespace Sunset
{
    Skeleton::Skeleton()
    {
    }

    Skeleton::~Skeleton()
    {
    }

    void Skeleton::Update(float dt)
    {
        std::vector<glm::mat4> BonesMatrix;

        // Do the trick
        for (auto& b : m_Bones)
        {

        }

        m_Buffer.Update(BonesMatrix);
    }
}
