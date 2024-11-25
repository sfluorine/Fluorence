#pragma once

#include "Common.h"

namespace FL
{

struct VertexElement {
    i32 count;
    GLenum type;
    GLboolean normalized;
    GLsizei offset;
};

class BufferLayout {
public:
    BufferLayout() = default;

    void
    PushLayout(i32 count, GLenum type, GLboolean normalized, GLsizei offset);

    usize GetStride() const
    {
        return m_stride;
    }

    std::span<const VertexElement> GetElements() const
    {
        return std::span(m_elements);
    };

private:
    GLsizei m_stride { 0 };
    std::vector<VertexElement> m_elements;
};

class VertexBuffer {
public:
    VertexBuffer(u32 id);

    VertexBuffer(const VertexBuffer& other) = delete;
    VertexBuffer& operator=(const VertexBuffer& other) = delete;

    VertexBuffer(VertexBuffer&& other);
    VertexBuffer& operator=(VertexBuffer&& other);

    void SetData(void* data, usize size) const;
    void Bind() const;

    void
    PushLayout(i32 count, GLenum type, GLboolean normalized, GLsizei offset)
    {
        m_layout.PushLayout(count, type, normalized, offset);
    }

    BufferLayout& GetLayout()
    {
        return m_layout;
    }

    const BufferLayout& GetLayout() const
    {
        return m_layout;
    }

public:
    static RefPtr<VertexBuffer> Create(usize size);
    static RefPtr<VertexBuffer> Create(void* data, usize size);

private:
    u32 m_id;
    BufferLayout m_layout;
};

class IndexBuffer {
public:
    IndexBuffer(u32 id);

    IndexBuffer(const IndexBuffer& other) = delete;
    IndexBuffer& operator=(const IndexBuffer& other) = delete;

    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;

    void Bind() const;

public:
    static RefPtr<IndexBuffer> Create(u32* indices, usize size);

private:
    u32 m_id;
};

} // namespace FL