//
// Created by sunvy on 29/08/2026.
//

#ifndef SUNSETENGINE_RENDERHANDLES_H
#define SUNSETENGINE_RENDERHANDLES_H

namespace Sunset
{
    struct ResourceHandle
    {
        constexpr ResourceHandle() = default;

        explicit constexpr ResourceHandle(const std::uint32_t resourceId)
            : id(resourceId)
        {
        }

        template <typename T>
        requires std::is_arithmetic_v<T>
        ResourceHandle& operator=(T value)
        {
            id = value;
            return *this;
        }

        [[nodiscard]] explicit operator bool() const
        {
            return id != 0;
        }

        operator unsigned int() const
        {
            return id;
        }

        unsigned int* operator&()
        {
            return &id;
        }

        std::uint32_t id = 0;
    };

    struct BufferHandle : ResourceHandle { using ResourceHandle::ResourceHandle; using ResourceHandle::operator=; };
    struct MeshHandle : ResourceHandle { using ResourceHandle::ResourceHandle; using ResourceHandle::operator=; };
    struct MaterialHandle : ResourceHandle { using ResourceHandle::ResourceHandle; using ResourceHandle::operator=; };
    struct ShaderHandle : ResourceHandle { using ResourceHandle::ResourceHandle; using ResourceHandle::operator=; };
}

#endif //SUNSETENGINE_RENDERHANDLES_H