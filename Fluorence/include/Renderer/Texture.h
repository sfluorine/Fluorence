#pragma once

#include "Common.h"

namespace FL
{

class Texture {
public:
    Texture(u32 id);

    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

    void Bind(u32 unit) const;

public:
    static RefPtr<Texture> LoadFromFile(fs::path path);

private:
    u32 m_id;
};

} // namespace FL