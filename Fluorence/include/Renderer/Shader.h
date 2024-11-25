#pragma once

#include "Common.h"

namespace FL
{

class Shader {
public:
    Shader() = default;

    Shader(u32 id);

    Shader(const Shader& other) = delete;
    Shader& operator=(const Shader& other) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    ~Shader();

    void Bind() const;
    void SetUniform1i(const char* name, i32 value) const;
    void SetUniform1iv(const char* name, i32 count, i32* values) const;
    void SetUniform3f(const char* name, const glm::vec3& value) const;
    void SetUniformMat4(const char* name, const f32* value) const;

public:
    static RefPtr<Shader> Load(const std::string& vertexSrc,
                               const std::string& fragmentSrc);

    static RefPtr<Shader> LoadFromFile(fs::path vertexPath,
                                       fs::path fragmentPath);

private:
    u32 m_id;
};

} // namespace FL
