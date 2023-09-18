#include "tilemap.hpp"

#include <jng/core/entry_point.hpp>
#include <jng/core/engine.hpp>
#include <jng/core/layer.hpp>
#include <jng/renderer/renderer2d.hpp>
#include <jng/renderer/renderer_api.hpp>
#include <jng/scene/camera.hpp>
#include <glm/glm.hpp>

static constexpr unsigned int WindowWidth = 800;
static constexpr unsigned int WindowHeight = 600;

class GameLayer :
	public jng::Layer
{
public:
    GameLayer() :
        m_tilemap{ "assets/untitled.tmx" }
    {
        m_camera.setViewportSize(WindowWidth, WindowHeight);
    }

    void onUpdate(float /*dt*/)
    {
        jng::RendererAPI::clear({ 0.1f, 0.1f, 0.1f });
        jng::Renderer2D::beginScene(m_camera.getVP(glm::mat4{ 1.f }));
        m_tilemap.render();
        jng::Renderer2D::endScene();
    }
private:
    jng::Camera m_camera;
    Tilemap m_tilemap;
};

class Game :
    public jng::Engine
{
public:
    Game() :
        Engine({
            "Just Game",
            WindowWidth,
            WindowHeight,
            RendererType::Renderer2D
            })
    {
        getLayerStack().pushLayer(new GameLayer{});
    }
};

jng::Engine* createApp() {
    return new Game{};
}
