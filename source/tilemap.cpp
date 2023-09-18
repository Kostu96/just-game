#include "tilemap.hpp"

#include "pugixml/pugixml.hpp"

#include <jng/debug/assert.hpp>

Tilemap::Tilemap(const std::filesystem::path& filename)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filename.c_str());

    JNG_USER_ASSERT(result.status == pugi::xml_parse_status::status_ok, "Could not load tilemap!");

    auto mapNode = doc.child("map");

    m_width = mapNode.attribute("width").as_uint();
    m_height = mapNode.attribute("height").as_uint();
    m_tileWidth = mapNode.attribute("tilewidth").as_uint();
    m_tileHeight = mapNode.attribute("tileheight").as_uint();
    m_tileData = new unsigned int[m_width * m_height];

    auto tilesetNode = mapNode.child("tileset");
    m_tileset = jng::makeRef<jng::Texture>();

    auto text = mapNode.child("layer").child("data").text().get();
    unsigned int valueStrPtr = 0, tileDataPtr = 0;
    while (*text != '\0')
    {
        char valueStr[4];
        char ch = *text++;
        switch (ch)
        {
        case '\n': continue;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            valueStr[valueStrPtr++] = ch;
            break;
        case ',':
            valueStr[valueStrPtr] = '\0';
            valueStrPtr = 0;
            if (tileDataPtr < m_width * m_height)
                m_tileData[tileDataPtr++] = std::atoi(valueStr);
            break;
        }
    }
}

Tilemap::~Tilemap()
{
    delete[] m_tileData;
}
