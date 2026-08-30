//
// Created by sunvy on 26/08/2026.
//

#include "Pipeline.h"

#include "Core/Application.h"
#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    std::shared_ptr<Pipeline> Pipeline::Create(const RenderState &state)
    {
        return Application::GetAPI()->CreatePipeline(state);
    }
} // Sunset