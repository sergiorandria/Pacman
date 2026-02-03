#pragma once

#include "e_IEntity.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <cstdint>

namespace g_PacmanEntityDecl {
class Wall : public g_PacmanEntityDecl::Entity {
public:
  Wall(std::uint16_t __wXidx, std::uint16_t __wYidx);
  bool _eIsAlive() const override { return false; }
};

} // namespace g_PacmanEntityDecl
