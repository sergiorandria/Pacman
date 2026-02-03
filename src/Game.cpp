#include "Game.h"
#include "e_Wall.h"
#include "g_GraphicsRenderer.h"
#include "g_MapBuilder.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdlib>
#include <memory>
#include <omp.h>

g_GameEngineInternal::Map g_GameEngineInternal::Game::map;
std::shared_mutex g_GameEngineInternal::Game::m_WindowMutexLock;

g_GameEngineInternal::Game::Game() {
  curGameState = g_GameEngineInternal::GameState::RUNNING;
  m_GraphicsRenderer =
      g_GameEngineInternal::Graphics::GraphicsRenderer::__pRenderGraphicsI();

  m_SharedWindowPtr = std::make_shared<sf::RenderWindow>();
  m_wRenderTexture = std::make_unique<sf::RenderTexture>();

  if (!m_SharedWindowPtr || !m_wRenderTexture) {
    std::abort();
  }

  m_window =
      new sf::RenderWindow(sf::VideoMode({winsize.x, winsize.y}), "Pacman");
  if (m_window == nullptr) {
    std::abort();
  }

  // We should call g_GraphicsRenderer once when creating
  // the Game object.  It should be lazy initialized..

  this->__call_RenderGraphicsOnce();
}

g_GameEngineInternal::Game::~Game() {
  if (m_window != nullptr) {
    m_SharedWindowPtr = nullptr;
    m_window->close();
  }
}

void g_GameEngineInternal::Game::__call_RenderGraphics() {}

// Lazy init
void g_GameEngineInternal::Game::__call_RenderGraphicsOnce() {
  if (!Game::map.empty()) {
    this->map = m_MapBuilder.createDefaultMap();
  }

  for (int y = 0; y < this->map.size(); ++y) {
    for (int x = 0; x < this->map[0].size(); ++x) {
      switch (map[y][x]) {
      case PACMAN_ENT_MAP_VALUE_WALL:
        auto wall = g_PacmanEntityDecl::Wall(x, y);
        // m_GraphicsRenderer->__pRenderGraphicsI()->renderObject();
        break;

        /*case PACMAN_ENT_MAP_VALUE_EXIT:
          break;

        case PACMAN_ENT_MAP_VALUE_PROT:

          break;

        case PACMAN_ENT_MAP_VALUE_ANTG:

          break;

        case PACMAN_ENT_MAP_VALUE_BG:
          break;

        case PACMAN_ENT_MAP_VALUE_PG:
          break;

        case PACMAN_ENT_MAP_VALUE_OG:
          break;

        case PACMAN_ENT_MAP_VALUE_RG:
          break;

        case PACMAN_ENT_MAP_VALUE_FOOD:
          break;*/

        // case PACMAN_ENT_MAP_VALUE_UNKWN:
        //  break;
      };
    }
  }
}

// Return the reference to the map object. The map
// object is a shared ressource between
g_GameEngineInternal::Map &g_GameEngineInternal::Game::getMap() { return map; }

void g_GameEngineInternal::Game::loop() {
#pragma omp parallel
  {
    while (m_window->isOpen()) {

#pragma omp parrallel for
      {
        while (const std::optional event = m_window->pollEvent()) {
          if (event->is<sf::Event::Closed>()) {
            m_window->close();
          }
        }
      }

      m_window->clear();
      this->__call_RenderGraphics();
      m_window->display();
    }
  }
}
