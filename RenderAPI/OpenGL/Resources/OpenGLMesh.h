//
// Created by sunvy on 26/08/2026.
//

#pragma once

#include "Render/Core/RenderHandles.h"
#include "Render/Resources/Mesh.h"

namespace Sunset
{
    class OpenGLMesh final : public Mesh
    {
    public:
        OpenGLMesh(const std::shared_ptr<Buffer>& vertexBuffer, const std::shared_ptr<Buffer>& indexBuffer, const VertexLayout& vertexLayout);

        ~OpenGLMesh() override;

        void Bind() override;

        [[nodiscard]] MeshHandle Get() const override;

    private:

        MeshHandle id{};
    };
} // Sunset