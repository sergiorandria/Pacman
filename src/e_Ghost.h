#pragma once

#include "e_AliveEntity.h"
#include "e_Pacman.h"
#include <SFML/Graphics.hpp>
#include <string>

namespace g_PacmanEntityDecl {
namespace g_GhostProperty {
enum class GhostColor { r, o, p, b };
}

class Ghost : public g_PacmanEntityDecl::AliveEntity, public sf::Drawable {
public:
  Ghost();
  Ghost(g_GhostProperty::GhostColor color);
  Ghost(const Ghost &objCopy);

  void e_MvLogic() noexcept override;
  
  void setPosition(std::uint16_t gridX, std::uint16_t gridY) {
    this->x = this->blockSize * gridX;
    this->y = this->blockSize * gridY;
    sprite_->setPosition({static_cast<float>(this->x), 
                       static_cast<float>(this->y)});
  }

  void setColor(g_GhostProperty::GhostColor color) {
    ghostColor_ = color;
    loadTexture();
  }

  void setScared(bool scared) {
    isScared_ = scared;
    if (scared) {
      if (!scaredTexture_.loadFromFile("../assets/scaredGhost.png")) { loadTexture(); };
      sprite_.emplace(scaredTexture_);
    } else {
      loadTexture();
    }
  }

private:
  sf::Texture texture_;
  sf::Texture scaredTexture_;
  std::optional<sf::Sprite> sprite_;
  g_GhostProperty::GhostColor ghostColor_;
  bool isScared_;

  // New per-ghost movement direction
  g_PacmanEntityDecl::Direction currentDirection_;
  
  void loadTexture() {
    std::string filename;
    switch (ghostColor_) {
      case g_GhostProperty::GhostColor::r: // Red
        filename = "../assets/redGhost.png";
        break;
      case g_GhostProperty::GhostColor::o: // Orange
        filename = "../assets/orangeGhost.png";
        break;
      case g_GhostProperty::GhostColor::p: // Pink
        filename = "../assets/pinkGhost.png";
        break;
      case g_GhostProperty::GhostColor::b: // Blue
        filename = "../assets/blueGhost.png";
        break;
    }
    
    if (texture_.loadFromFile(filename)) {
      sprite_.emplace(texture_);
      
      // Scale sprite to fit block size
      float scaleX = static_cast<float>(blockSize) / texture_.getSize().x;
      float scaleY = static_cast<float>(blockSize) / texture_.getSize().y;
      sprite_->setScale({scaleX, scaleY});
    }
  }

  // sf::Drawable override
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    target.draw(*sprite_, states);
  }
};
} // namespace g_PacmanEntityDecl
