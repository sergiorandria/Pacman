#pragma once

#include "e_Ghost.h"
#include "e_Pacman.h"
#include "e_Wall.h"
#include "g_Constant.h"

#include <SFML/Graphics.hpp>

#include <cstdint>
#include <mutex>
#include <string>
#include <vector>

namespace g_GameEngineInternal {
typedef std::vector<std::string> Map;

enum VALID_MAP_VALUE {
  PACMAN_ENT_MAP_VALUE_WALL = 0x58,
  PACMAN_ENT_MAP_VALUE_EXIT = 0x4f,
  PACMAN_ENT_MAP_VALUE_PROT = 0x50,
  PACMAN_ENT_MAP_VALUE_ANTG = 0x7f,
  PACMAN_ENT_MAP_VALUE_BG = 0x62,
  PACMAN_ENT_MAP_VALUE_PG = 0x70,
  PACMAN_ENT_MAP_VALUE_OG = 0x6f,
  PACMAN_ENT_MAP_VALUE_RG = 0x72,
  PACMAN_ENT_MAP_VALUE_FOOD = 0x20,
  PACMAN_ENT_MAP_VALUE_UNKWN = 0x00,
};

// Can be very expensive
class MapBuilder {
private:
  volatile std::vector<sf::Image> gImages;

  std::shared_ptr<MapBuilder> resPtr;
  std::once_flag resFlag;

public:
  MapBuilder();
  MapBuilder(void *instance, const Map &map);

  virtual g_PacmanEntityDecl::Wall *createWall_(std::uint16_t __wXidx,
                                                std::uint16_t __wYidx) {
    return new g_PacmanEntityDecl::Wall(__wXidx, __wYidx);
  }

  virtual void createWall(std::uint16_t __wXidx, std::uint16_t __wYidx) {}

  virtual g_PacmanEntityDecl::Pacman *createPacman_() {
    return g_PacmanEntityDecl::Pacman::Instance();
  }

  virtual g_PacmanEntityDecl::Ghost *createGhost_() {
    return new g_PacmanEntityDecl::Ghost();
  }

  virtual void createGhost() {}

  virtual g_GameEngineInternal::Map createDefaultMap() {
    return g_GameEngineInternal::__default_map;
  }
};
} // namespace g_GameEngineInternal
