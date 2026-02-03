#include "g_MapBuilder.h"

g_GameEngineInternal::MapBuilder::MapBuilder() {}

g_GameEngineInternal::MapBuilder::MapBuilder(void *instance, const Map &map) {
  for (const auto &row : map) {
    for (const char &c : row) {

      switch (c) {
      case PACMAN_ENT_MAP_VALUE_WALL:
        // Walls are created in Game.cpp
        break;

      case PACMAN_ENT_MAP_VALUE_EXIT:
        // Exit points (O in the map)
        break;

      case PACMAN_ENT_MAP_VALUE_PROT:
        // Pacman protagonist
        break;

      case PACMAN_ENT_MAP_VALUE_ANTG:
        // Antagonist (unused)
        break;

      case PACMAN_ENT_MAP_VALUE_BG:
        // Blue ghost
        break;

      case PACMAN_ENT_MAP_VALUE_PG:
        // Pink ghost
        break;

      case PACMAN_ENT_MAP_VALUE_OG:
        // Orange ghost
        break;

      case PACMAN_ENT_MAP_VALUE_RG:
        // Red ghost
        break;

      case PACMAN_ENT_MAP_VALUE_FOOD:
        // Food pellets (spaces in the map)
        break;

      case PACMAN_ENT_MAP_VALUE_UNKWN:
        // Unknown
        break;
      };
    }
  }
}
