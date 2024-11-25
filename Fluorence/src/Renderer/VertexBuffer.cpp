#include "Renderer/VertexBuffer.h"

namespace FL
{

static GLsizei GetOpenGLSizeType(GLenum type)
{
    switch (type) {
    case GL_FLOAT:
        return sizeof(f32);
    default:
        spdlog::error("invalid type for GetOpenGLSizeType!");
        return 0;
    }
}

void BufferLayout::PushLayout(i32 count,
                              GLenum type,
                              GLboolean normalized,
                              GLsizei offset)
{
    m_elements.push_back({
        .count = count,
        .type = type,
        .normalized = normalized,
        .offset = offset,
    });

    m_stride += count * GetOpenGLSizeType(type);
}

VertexBuffer::VertexBuffer(u32 id)
    : m_id(id)
{
}

VertexBuffer::VertexBuffer(VertexBuffer&& other)
    : m_id(other.m_id)
{
    other.m_id = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other)
{
    m_id = other.m_id;
    other.m_id = 0;
    return *this;
}

void VertexBuffer::SetData(void* data, usize size) const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

RefPtr<VertexBuffer> VertexBuffer::Create(usize size)
{
    u32 id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

    return MakeRef<VertexBuffer>(id);
}

RefPtr<VertexBuffer> VertexBuffer::Create(void* data, usize size)
{
    u32 id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    return MakeRef<VertexBuffer>(id);
}

IndexBuffer::IndexBuffer(u32 id)
    : m_id(id)
{
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

RefPtr<IndexBuffer> IndexBuffer::Create(u32* indices, usize size)
{
    u32 id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);

    return MakeRef<IndexBuffer>(id);
}

} // namespace FL