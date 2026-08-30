//
// Created by sunvy on 15/12/2025.
//

#include "Shader.h"

#include "Core/Application.h"
#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    std::unique_ptr<Shader> Shader::CreateShader(const std::string_view &vertShader, const std::string_view &fragShader)
    {
        return Application::GetAPI()->CreateShader(vertShader, fragShader);
    }
}
