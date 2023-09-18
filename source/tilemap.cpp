#include "tilemap.hpp"

#include "pugixml/pugixml.hpp"

#include <jng/debug/assert.hpp>
#include <jng/renderer/renderer2d.hpp>

Tilemap::Tilemap(const std::filesystem::path& filename)
{
    pugi::xml_document tmxDoc;
    pugi::xml_parse_result result = tmxDoc.load_file(filename.c_str());
    JNG_USER_ASSERT(result.status == pugi::xml_parse_status::status_ok, "Could not load tilemap TMX file!");

    auto mapNode = tmxDoc.child("map");
    m_width = mapNode.attribute("width").as_uint();
    m_height = mapNode.attribute("height").as_uint();
    m_tileWidth = mapNode.attribute("tilewidth").as_uint();
    m_tileHeight = mapNode.attribute("tileheight").as_uint();
    m_tileData = new unsigned int[m_width * m_height];

    auto tsxFilename = filename.parent_path() / mapNode.child("tileset").attribute("source").as_string();

    pugi::xml_document tsxDoc;
    result = tsxDoc.load_file(tsxFilename.c_str());
    JNG_USER_ASSERT(result.status == pugi::xml_parse_status::status_ok, "Could not load tilemap TSX file!");

    auto tilesetFilename = filename.parent_path() / tsxDoc.child("tileset").child("image").attribute("source").as_string();
    m_tileset = jng::makeRef<jng::Texture>(tilesetFilename.string().c_str()); // TODO: casting to string wont be needed when Texture will take fs::path

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

void Tilemap::render() const
{
    static constexpr glm::vec3 vertexPositions[] = {
        { -1.f, -1.f, 0.0f },
        {  1.f, -1.f, 0.0f },
        {  1.f,  1.f, 0.0f },
        { -1.f,  1.f, 0.0f }
    };
    static constexpr glm::vec2 texCoords[] = {
        { 0.f, 0.f },
        { 1.f, 0.f },
        { 1.f, 1.f },
        { 0.f, 1.f }
    };

    static const jng::Renderer2D::DrawQuadProperties props{
        vertexPositions,
        texCoords,
        m_tileset,
        glm::packUnorm4x8({ 1.f, 1.f, 1.f, 1.f })
    };
    jng::Renderer2D::drawQuad(props);
}
