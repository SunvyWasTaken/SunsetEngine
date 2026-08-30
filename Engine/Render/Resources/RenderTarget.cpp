//
// Created by sunvy on 26/08/2026.
//

#include "RenderTarget.h"

#include "Core/Application.h"
#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    std::shared_ptr<RenderTarget> RenderTarget::Create(const RenderTargetSpecification &spec)
    {
        return Application::GetAPI()->CreateFramebuffer(spec);
    }
} // Sunset