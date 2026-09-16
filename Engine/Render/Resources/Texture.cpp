//
// Created by sunvy on 19/12/2025.
//

#include "Texture.h"

#include "Core/Application.h"
#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    std::unique_ptr<Texture> Texture::Create(const TextureDescription& desc)
    {
        return Application::GetAPI()->CreateTexture(desc);
    }
} // Sunset
