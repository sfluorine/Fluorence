#include "3D/Model.h"

namespace FL
{

Model::Model(Vector<Vertex3D> vertices)
{
    m_vertices = std::move(vertices);

    m_vao = FL::VertexArray::Create();
    m_vbo = FL::VertexBuffer::Create(m_vertices.data(),
                                     m_vertices.size() * sizeof(Vertex3D));

    m_vbo->PushLayout(3, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, position));
    m_vbo->PushLayout(3, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, normal));
    m_vbo->PushLayout(2, GL_FLOAT, GL_FALSE, offsetof(Vertex3D, uv));

    m_vao->SetVertexBuffer(m_vbo);

    m_vao->Unbind();
}

void Model::Draw(const RefPtr<Shader>& shader) const
{
    shader->Bind();
    m_vao->Bind();

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size() * 3);

    m_vao->Unbind();
}

RefPtr<Model> Model::LoadFromFile(fs::path path)
{
    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(path.string())) {
        if (!reader.Error().empty()) {
            spdlog::error("{}", reader.Error());
            return nullptr;
        }
    }

    if (!reader.Warning().empty()) {
        spdlog::warn("{}", reader.Warning());
    }

    Vector<Vertex3D> vertices;

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    for (u32 s = 0; s < shapes.size(); s++) {
        u32 indexOffset = 0;

        for (u32 f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            u32 fv = shapes[s].mesh.num_face_vertices[f];

            for (u32 v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];

                tinyobj::real_t x = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t y = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t z = attrib.vertices[3 * idx.vertex_index + 2];

                glm::vec3 position(x, y, z);
                glm::vec3 normal(0.0f);
                glm::vec2 uv(0.0f);

                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx
                        = attrib.normals[3 * idx.normal_index + 0];
                    tinyobj::real_t ny
                        = attrib.normals[3 * idx.normal_index + 1];
                    tinyobj::real_t nz
                        = attrib.normals[3 * idx.normal_index + 2];

                    normal = glm::vec3(nx, ny, nz);
                }

                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t s
                        = attrib.texcoords[2 * idx.texcoord_index + 0];
                    tinyobj::real_t t
                        = attrib.texcoords[2 * idx.texcoord_index + 1];

                    uv = glm::vec2(s, t);
                }

                vertices.push_back(
                    { .position = position, .normal = normal, .uv = uv });
            }

            indexOffset += fv;
        }
    }

    return MakeRef<Model>(std::move(vertices));
}

}