//
// Created by sunvy on 14/06/2026.
//

#include "SkeletalMesh.h"
#include "Render/BufferObject/VertexArray.h"

namespace Sunset
{
    SkeletalMesh::SkeletalMesh(std::unique_ptr<VertexArray>& vao)
        : Mesh(vao)
        , skeleton()
    {
    }

    SkeletalMesh::~SkeletalMesh()
    {
    }

    void SkeletalMesh::Update(float dt)
    {
        skeleton.Update(dt);
    }
} // Sunset