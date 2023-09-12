#include <jng/core/entry_point.hpp>
#include <jng/core/engine.hpp>
#include <jng/core/layer.hpp>

static constexpr unsigned int WindowWidth = 800;
static constexpr unsigned int WindowHeight = 600;

class GameLayer :
	public jng::Layer
{
public:

private:

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
