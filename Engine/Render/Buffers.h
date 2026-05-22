//
// Created by sunvy on 07/02/2026.
//

#ifndef SUNSETCRAFT_BUFFERS_H
#define SUNSETCRAFT_BUFFERS_H

namespace SunsetEngine
{
    enum class ShaderDataType {
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        UInt, Bool
    };

    struct BufferElement final
    {
        BufferElement(ShaderDataType type, const std::string_view& name, bool normalized = false);

        [[nodiscard]] bool IsInt() const;
        [[nodiscard]] int Count() const;
        [[nodiscard]] int Type() const;

        std::string name;
        ShaderDataType type;
        size_t size;
        size_t offset;
        bool normalized;
        uint32_t divisor;
    };

    class BufferLayout final
    {
    public:
        BufferLayout(const std::initializer_list<BufferElement>& list);

        const std::vector<BufferElement>& operator()() const;

        [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const;
        [[nodiscard]] std::vector<BufferElement>::const_iterator end() const;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t GetStride() const;

    private:
        std::vector<BufferElement> m_Elements;
        size_t m_Stride;
    };

    // VBO
    class VertexBuffer
    {
    public:
        VertexBuffer(const void* data, size_t size, size_t dataSize);
        virtual ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

        void SetLayout(const std::initializer_list<BufferElement>& elements);
        BufferLayout GetLayout() const;
        size_t GetSize() const;
    private:
        uint32_t m_Id;
        BufferLayout m_Layout;
        size_t m_Size;
    };

    // EBO
    class IndiceBuffer
    {
    public:
        explicit IndiceBuffer(const std::vector<uint32_t>& indices);
        virtual ~IndiceBuffer();
        void Bind() const;
        void Unbind() const;
        size_t GetCount() const;
    private:
        uint32_t m_Id;
        size_t m_Count;
    };
}

#endif //SUNSETCRAFT_BUFFERS_H