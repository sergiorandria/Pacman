#pragma once

#include <cstdint>

namespace g_PacmanEntityDecl {
class Entity {

public:
  virtual bool _eIsAlive() const = 0;

protected:
  std::uint16_t x;
  std::uint16_t y;

  static constexpr int blockSize = 50;
};
} // namespace g_PacmanEntityDecl
