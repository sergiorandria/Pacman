#pragma once

#include "e_AliveEntity.h"

#include <SFML/Graphics.hpp>
#include <mutex>
#include <string>

namespace g_PacmanEntityDecl {

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Pacman : public g_PacmanEntityDecl::AliveEntity, public sf::Drawable {
private:
  static Pacman *instance;
  static std::mutex p_MutexLock;

  sf::Texture textureUp_;
  sf::Texture textureDown_;
  sf::Texture textureLeft_;
  sf::Texture textureRight_;
  std::optional<sf::Sprite> sprite_;
  
  Direction currentDirection_;

  Pacman() : currentDirection_(Direction::RIGHT) {
    // Load all direction textures
    if (!textureUp_.loadFromFile("../assets/pacmanUp.png")) {
      // Fallback or error handling
    }
    if (!textureDown_.loadFromFile("../assets/pacmanDown.png")) {
      // Fallback or error handling
    }
    if (!textureLeft_.loadFromFile("../assets/pacmanLeft.png")) {
      // Fallback or error handling
    }
    if (!textureRight_.loadFromFile("../assets/pacmanRight.png")) {
      // Fallback or error handling
    }
    
    // Start with right-facing texture
    sprite_.emplace(textureRight_);
    
    // Scale sprite to fit block size if needed
    float scaleX = static_cast<float>(blockSize) / textureRight_.getSize().x;
    float scaleY = static_cast<float>(blockSize) / textureRight_.getSize().y;
    sprite_->setScale({scaleX, scaleY});
  }

  // sf::Drawable override
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(*sprite_, states);
  }

public:
  Pacman(const Pacman &cpObject) = delete;
  static Pacman *Instance() noexcept;

  void e_MvLogic() noexcept override;
  
  void setPosition(std::uint16_t gridX, std::uint16_t gridY) {
    this->x = this->blockSize * gridX;
    this->y = this->blockSize * gridY;
    sprite_->setPosition({static_cast<float>(this->x), static_cast<float>(this->y)});
  }

  void setDirection(Direction dir) {
    currentDirection_ = dir;
    switch (dir) {
      case Direction::UP:
        sprite_.emplace(textureUp_);
        break;
      case Direction::DOWN:
        sprite_.emplace(textureDown_);
        break;
      case Direction::LEFT:
        sprite_.emplace(textureLeft_);
        break;
      case Direction::RIGHT:
        sprite_.emplace(textureRight_);
        break;
    }
  }

  Direction getDirection() const { return currentDirection_; }
};
} // namespace g_PacmanEntityDecl
