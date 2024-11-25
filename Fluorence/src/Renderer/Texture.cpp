#include "Renderer/Texture.h"

namespace FL
{

Texture::Texture(u32 id)
    : m_id(id)
{
}

Texture::Texture(Texture&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

void Texture::Bind(u32 unit) const
{
    glBindTextureUnit(unit, m_id);
}

RefPtr<Texture> Texture::LoadFromFile(fs::path path)
{
    auto path_string = path.string();

    if (!fs::exists(path)) {
        spdlog::error(
            "failed to load texture from '{}': no such file or directory!",
            path_string);
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(true);

    auto width = 0;
    auto height = 0;
    auto nrChannels = 0;

    u8* data = stbi_load(
        path_string.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

    if (!data) {
        spdlog::error("failed to load texture from '{}'!", path_string);
        return nullptr;
    }

    u32 id;
    glCreateTextures(GL_TEXTURE_2D, 1, &id);

    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(id, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(
        id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    glGenerateTextureMipmap(id);

    return MakeRef<Texture>(id);
}

} // namespace FL