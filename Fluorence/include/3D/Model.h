#pragma once

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"

namespace FL
{

struct Vertex3D {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

class Model {
public:
    Model(Vector<Vertex3D> vertices);

    Model(const Model& other) = delete;
    Model& operator=(const Model& other) = delete;

    Model(Model&& other) = delete;
    Model& operator=(Model&& other) = delete;

    void Draw(const RefPtr<Shader>& shader) const;

public:
    static RefPtr<Model> LoadFromFile(fs::path path);

private:
    Vector<Vertex3D> m_vertices;
    RefPtr<VertexArray> m_vao;
    RefPtr<VertexBuffer> m_vbo;
};

}