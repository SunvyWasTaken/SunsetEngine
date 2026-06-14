//
// Created by sunvy on 14/06/2026.
//

#pragma once

#include "Mesh.h"
#include "Skeleton.h"

namespace Sunset
{
    class SkeletalMesh final : public Mesh
    {
    public:
        explicit SkeletalMesh(std::unique_ptr<VertexArray>& vao);
        ~SkeletalMesh() override;

        void Update(float dt);

        Skeleton skeleton;
    };
} // Sunset