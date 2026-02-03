#pragma once

// Globalization
#include <string>
#include <vector>
#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef UNICODE
#define UNICODE
#endif

#ifndef __cpp_unicode_characters
#define __cpp_unicode_characters
#endif

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>
#include <memory>
#include <shared_mutex>

#include "g_GraphicsRenderer.h"
#include "g_MapBuilder.h"
#include "e_Pacman.h"
#include "e_Ghost.h"
#include "e_Food.h"

namespace g_GameEngineInternal {

enum class GameState { PAUSED, RUNNING };

constexpr int imgCount = 13;

class Game {
  static constexpr int bpp = 255;
  static constexpr sf::Vector2u winsize = {950, 1050};

  friend class g_GameEngineInternal::MapBuilder;
  friend class g_GameEngineInternal::Graphics::GraphicsRenderer;

public:
  Game();
  Game(const Game &&gInstance);
  ~Game();

  void __call_RenderGraphics();
  void __call_RenderGraphicsOnce();

  static Map &getMap();

  void loop();

private:
  g_GameEngineInternal::MapBuilder m_MapBuilder;
  g_GameEngineInternal::Graphics::GraphicsRenderer *m_GraphicsRenderer;

  g_GameEngineInternal::GameState curGameState;

  sf::RenderWindow *window;

  std::shared_ptr<sf::RenderWindow> m_SharedWindowPtr;
  std::unique_ptr<sf::RenderTexture> m_wRenderTexture;
  static std::shared_mutex m_WindowMutexLock;

  static g_GameEngineInternal::Map map;

  // Store created entities
  std::vector<std::unique_ptr<g_PacmanEntityDecl::Wall>> m_walls;
  g_PacmanEntityDecl::Pacman* m_pacman;
  std::vector<std::unique_ptr<g_PacmanEntityDecl::Ghost>> m_ghosts;
  std::vector<std::unique_ptr<g_PacmanEntityDecl::Food>> m_food;
  
  // Game state
  int score_;
  int lives_;
};
} // namespace g_GameEngineInternal
