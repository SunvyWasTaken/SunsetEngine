//
// Created by sunvy on 26/08/2026.
//

#ifndef SUNSETENGINE_BUFFER_H
#define SUNSETENGINE_BUFFER_H

namespace Sunset
{
    enum class BufferType
    {
        Vertex,
        Index,
        Uniform
    };

    class Buffer
    {
    public:
        static std::shared_ptr<Buffer> Create(const BufferType& type);
    public:
        virtual ~Buffer() = default;

        virtual void Bind() = 0;
        virtual void SetData(const void* data, std::size_t typeSize, std::size_t size) = 0;
        [[nodiscard]] virtual std::size_t Count() const = 0;
    };
}

#endif //SUNSETENGINE_BUFFER_H