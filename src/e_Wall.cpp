#include "e_Wall.h"
#include <cstdint>

g_PacmanEntityDecl::Wall::Wall(std::uint16_t __wXidx, std::uint16_t __wYidx) {
  this->x = this->blockSize * __wXidx;
  this->y = this->blockSize * __wYidx;
}
