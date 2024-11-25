#include "Renderer/Shader.h"

namespace FL
{

Shader::Shader(u32 id)
    : m_id(id)
{
}

Shader::Shader(Shader&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept
{
    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::Bind() const
{
    glUseProgram(m_id);
}

void Shader::SetUniform1i(const char* name, i32 value) const
{
    glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::SetUniform1iv(const char* name, i32 count, i32* values) const
{
    glUniform1iv(glGetUniformLocation(m_id, name), count, values);
}

void Shader::SetUniform3f(const char* name, const glm::vec3& value) const
{
    glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
}

void Shader::SetUniformMat4(const char* name, const f32* value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, value);
}

RefPtr<Shader> Shader::Load(const std::string& vertexSrc,
                            const std::string& fragmentSrc)
{
    const char* vertexCode = vertexSrc.c_str();
    const char* fragmentCode = fragmentSrc.c_str();

    int success {};

    u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetShaderInfoLog(vertexShader, 255, nullptr, log);

        std::for_each(std::begin(log), std::end(log), [](auto& c) {
            if (c == '\n')
                c = '\0';
        });

        spdlog::error("vertex shader compile error: {}", log);
        return nullptr;
    }

    u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetShaderInfoLog(fragmentShader, 255, nullptr, log);

        std::for_each(std::begin(log), std::end(log), [](auto& c) {
            if (c == '\n')
                c = '\0';
        });

        spdlog::error("fragment shader compile error: {}", log);
        return nullptr;
    }

    u32 program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[255];
        glGetProgramInfoLog(program, 255, nullptr, log);

        std::for_each(std::begin(log), std::end(log), [](auto& c) {
            if (c == '\n')
                c = '\0';
        });

        spdlog::error("shader linking error: {}", log);
        return nullptr;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return MakeRef<Shader>(program);
}

RefPtr<Shader> Shader::LoadFromFile(fs::path vertexPath, fs::path fragmentPath)
{
    if (!fs::exists(vertexPath)) {
        spdlog::error("could not find vertex shader file '{}'",
                      vertexPath.string());
        return nullptr;
    }

    if (!fs::exists(fragmentPath)) {
        spdlog::error("could not find fragment shader file '{}'",
                      fragmentPath.string());
        return nullptr;
    }

    std::ifstream vertexStream(vertexPath);
    std::stringstream vertexSstream;

    std::ifstream fragmentStream(fragmentPath);
    std::stringstream fragmentSstream;

    std::string vertexSource;
    std::string fragmentSource;

    if (!vertexStream) {
        spdlog::critical("INTERNAL ERROR: could not open file '{}'",
                         vertexPath.string());
        return nullptr;
    }

    vertexSstream << vertexStream.rdbuf();
    vertexSource = vertexSstream.str();

    if (!fragmentStream) {
        spdlog::critical("INTERNAL ERROR: could not open file '{}'",
                         fragmentPath.string());
        return nullptr;
    }

    fragmentSstream << fragmentStream.rdbuf();
    fragmentSource = fragmentSstream.str();

    auto shader = Shader::Load(vertexSource, fragmentSource);
    if (!shader) {
        return nullptr;
    }

    return shader;
}

} // namespace FL