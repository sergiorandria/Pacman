#include "g_MapBuilder.h"

g_GameEngineInternal::MapBuilder::MapBuilder() {}

g_GameEngineInternal::MapBuilder::MapBuilder(void *instance, const Map &map) {
  for (const auto &row : map) {
    for (const char &c : row) {

      switch (c) {
      case PACMAN_ENT_MAP_VALUE_WALL:
        break;

      case PACMAN_ENT_MAP_VALUE_EXIT:
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
        break;

      case PACMAN_ENT_MAP_VALUE_UNKWN:
        break;
      };
    }
  }
}
