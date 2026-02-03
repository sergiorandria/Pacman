#pragma once

#include "e_IEntity.h"
#include <SFML/Graphics.hpp>
#include <cstdint>

namespace g_PacmanEntityDecl {

    class Food : public g_PacmanEntityDecl::Entity, public sf::Drawable {
        public:
            Food(std::uint16_t gridX, std::uint16_t gridY, bool isPowerPellet = false) 
                : isPowerPellet_(isPowerPellet), isEaten_(false) 
            {
                this->x = this->blockSize * gridX;
                this->y = this->blockSize * gridY;

                const float __x = static_cast<float>(this->x); 
                const float __y = static_cast<float>(this->y); 

                if (isPowerPellet) {
                    // Load power pellet texture
                    if (texture_.loadFromFile("../assets/powerFood.png")) {
                        sprite_.emplace(texture_);
                        float scaleX = static_cast<float>(blockSize) / texture_.getSize().x;
                        float scaleY = static_cast<float>(blockSize) / texture_.getSize().y;
                        sprite_->setScale({scaleX, scaleY});
                        sprite_->setPosition({ __x, __y});
                    } else {
                        // Fallback to large circle
                        // Should not be used 
                        // The best option is to return error and quit.
                        circle_.setRadius(8.0f);
                        circle_.setFillColor(sf::Color::Yellow);
                        circle_.setPosition({ __x + blockSize / 2.0f - 8.0f,
                                __y + blockSize / 2.0f - 8.0f});
                    }
                } else {
                    // Regular food pellet - small dot
                    circle_.setRadius(3.0f);
                    circle_.setFillColor(sf::Color::Yellow);
                    circle_.setPosition({ __x + blockSize / 2.0f - 3.0f,
                            __y + blockSize / 2.0f - 3.0f});
                }
            }

            bool _eIsAlive() const override { return false; }

            bool isEaten() const { return isEaten_; }
            void setEaten(bool eaten) { isEaten_ = eaten; }

            bool isPowerPellet() const { return isPowerPellet_; }

        private:
            bool isPowerPellet_;
            bool isEaten_;
            std::bool_constant<true> useTexture_;

            // Pretty messy around
            // Migrate from SFML 2 to SFML 3
            sf::Texture texture_;
            std::optional<sf::Sprite> sprite_;
            sf::CircleShape circle_;

            void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
                if (!isEaten_) {
                    if (sprite_) {
                        target.draw(*sprite_, states);
                    } 
                }
            }
    };

} // namespace g_PacmanEntityDecl
