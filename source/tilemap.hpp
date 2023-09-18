#pragma once
#include <jng/renderer/texture.hpp>
#include <filesystem>

class Tilemap
{
public:
    explicit Tilemap(const std::filesystem::path& filename);
    ~Tilemap();
private:
    unsigned int m_width, m_height, m_tileWidth, m_tileHeight;
    unsigned int* m_tileData = nullptr;
    jng::Ref<jng::Texture> m_tileset;
};
