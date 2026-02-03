#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <mutex>

namespace g_GameEngineInternal {
namespace Graphics {
class GraphicsRenderer {
private:
  static GraphicsRenderer *graphics;
  static std::mutex graphicsLock;

  GraphicsRenderer() {}

public:
  GraphicsRenderer(const GraphicsRenderer &gCopy) = delete;

  static GraphicsRenderer *__pRenderGraphicsI() {
    if (graphics == nullptr) {
      std::lock_guard<std::mutex> lock(graphicsLock);
      if (graphics == nullptr) {
        graphics = new GraphicsRenderer();
      }
    }
    return graphics;
  }

  virtual void renderObject(sf::RenderTarget &target, const sf::Drawable &obj);
};

} // namespace Graphics
} // namespace g_GameEngineInternal
