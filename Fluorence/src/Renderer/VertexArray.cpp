#include "Renderer/VertexArray.h"

namespace FL
{

VertexArray::VertexArray(u32 id)
    : m_id(id)
    , m_index(0)
{
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_id(other.m_id)
    , m_index(other.m_index)
{
    other.m_id = 0;
    other.m_index = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    m_id = other.m_id;
    m_index = other.m_index;
    other.m_id = 0;
    other.m_index = 0;

    return *this;
}

void VertexArray::SetVertexBuffer(const RefPtr<VertexBuffer>& vb)
{
    glBindVertexArray(m_id);
    vb->Bind();

    auto& layout = vb->GetLayout();
    auto stride = layout.GetStride();

    for (auto& element : layout.GetElements()) {
        glEnableVertexAttribArray(m_index);
        glVertexAttribPointer(
            m_index++,
            element.count,
            element.type,
            element.normalized,
            static_cast<i32>(stride),
            reinterpret_cast<void*>(static_cast<intptr_t>(element.offset)));
    }
}

void VertexArray::SetIndexBuffer(const RefPtr<IndexBuffer>& ib)
{
    glBindVertexArray(m_id);
    ib->Bind();
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

RefPtr<VertexArray> VertexArray::Create()
{
    u32 id;
    glGenVertexArrays(1, &id);

    return MakeRef<VertexArray>(id);
}

} // namespace FL