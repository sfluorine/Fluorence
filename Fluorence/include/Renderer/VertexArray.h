#pragma once

#include "Common.h"
#include "VertexBuffer.h"

namespace FL
{

class VertexArray {
public:
    VertexArray(u32 id);

    VertexArray(const VertexArray& other) = delete;
    VertexArray& operator=(const VertexArray& other) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void SetVertexBuffer(const RefPtr<VertexBuffer>& vb);
    void SetIndexBuffer(const RefPtr<IndexBuffer>& ib);
    void Bind() const;
    void Unbind() const;

public:
    static RefPtr<VertexArray> Create();

private:
    u32 m_id;
    u32 m_index;
};

} // namespace FL