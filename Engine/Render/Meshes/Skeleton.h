//
// Created by sunvy on 14/06/2026.
//

#pragma once

#include "Bones.h"
#include "Render/BufferObject/UniformBuffer.h"

namespace Sunset
{

    using Bones = std::vector<Bone>;

    class Skeleton final
    {
    public:
        Skeleton();

        ~Skeleton();
        
        void Update(float dt);

    private:
        Bones m_Bones;
        UniformBuffer<glm::mat4> m_Buffer;
    };
}
