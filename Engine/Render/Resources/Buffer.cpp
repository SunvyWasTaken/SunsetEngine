//
// Created by sunvy on 30/08/2026.
//

#include "Buffer.h"

#include "Core/Application.h"
#include "Render/Core/RenderAPI.h"

namespace Sunset
{
    std::shared_ptr<Buffer> Buffer::Create(const BufferType &type)
    {
        return Application::GetAPI()->CreateBuffer(type);
    }
}
