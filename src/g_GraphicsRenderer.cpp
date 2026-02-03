#include "g_GraphicsRenderer.h"
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <mutex>

g_GameEngineInternal::Graphics::GraphicsRenderer
    *g_GameEngineInternal::Graphics::GraphicsRenderer::graphics;
std::mutex g_GameEngineInternal::Graphics::GraphicsRenderer::graphicsLock;

void g_GameEngineInternal::Graphics::GraphicsRenderer::renderObject(
    sf::RenderTarget &target, const sf::Drawable &obj) {
  auto gr = Graphics::GraphicsRenderer::__pRenderGraphicsI();

  std::lock_guard<std::mutex> lock(graphicsLock);
  target.draw(obj, sf::RenderStates::Default);
}
