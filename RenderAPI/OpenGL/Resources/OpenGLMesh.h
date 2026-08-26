//
// Created by sunvy on 26/08/2026.
//

#pragma once

#include "Render/Resources/Mesh.h"

namespace Sunset
{
    class OpenGLMesh final : public Mesh
    {
    public:
        OpenGLMesh(const std::shared_ptr<Buffer>& vertexBuffer, const std::shared_ptr<Buffer>& indexBuffer, const VertexLayout& vertexLayout);

        ~OpenGLMesh() override;

        void Bind() override;

    private:

        std::uint32_t id;
    };
} // Sunset