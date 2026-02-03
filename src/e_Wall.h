#pragma once

#include "e_IEntity.h"
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <memory>

namespace g_PacmanEntityDecl {
    class Wall : public g_PacmanEntityDecl::Entity, public sf::Drawable {
        public:
            Wall(std::uint16_t __wXidx, std::uint16_t __wYidx) {
                // Entity coords are in pixels (blockSize * index)
                this->x = this->blockSize * __wXidx;
                this->y = this->blockSize * __wYidx;

                // Load wall texture
                if (texture_.loadFromFile("../assets/wall.png")) {
                    sprite_.emplace(texture_);

                    // Scale sprite to fit block size
                    float scaleX = static_cast<float>(blockSize) / texture_.getSize().x;
                    float scaleY = static_cast<float>(blockSize) / texture_.getSize().y;
                    sprite_->setScale({scaleX, scaleY});

                    sprite_->setPosition({static_cast<float>(this->x), static_cast<float>(this->y)});
                } 
            }

            bool _eIsAlive() const override { return false; }

        private:
            sf::Texture texture_;
            std::optional<sf::Sprite> sprite_;
            sf::RectangleShape shape_;
            bool useTexture_ = true;

            // sf::Drawable override
            void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
                if (useTexture_) {
                    target.draw(*sprite_, states);
                } else {
                    target.draw(shape_, states);
                }
            }
    };
} // namespace g_PacmanEntityDecl
