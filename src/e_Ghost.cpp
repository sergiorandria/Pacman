#include <random> 
#include <chrono> 
#include "Game.h"
#include "e_Ghost.h"

g_PacmanEntityDecl::Ghost::Ghost() 
    : ghostColor_(g_GhostProperty::GhostColor::r), isScared_(false) {
        loadTexture();
    }

g_PacmanEntityDecl::Ghost::Ghost(g_GhostProperty::GhostColor color) 
    : ghostColor_(color), isScared_(false) {
        loadTexture();
    }

g_PacmanEntityDecl::Ghost::Ghost(const Ghost &objCopy) { 
    *this = objCopy; 
}

void g_PacmanEntityDecl::Ghost::e_MvLogic() noexcept {
    // Simple random-walk movement with wall collision:
    // - Occasionally change direction randomly
    // - Before moving, check the target map cell; if it's a wall ('X'), pick
    //   another direction (up to 4 tries). If all directions blocked, do not move.
    // - Move a small number of pixels per frame in currentDirection_
    // - If scared, move slower
    static thread_local std::mt19937 rng(
            static_cast<unsigned int>(
                std::chrono::high_resolution_clock::now().time_since_epoch().count()
                )
            );
    std::uniform_int_distribution<int> changeDist(0, 99);
    std::uniform_int_distribution<int> dirDist(0, 3);

    // Get the game map for collision checking
    auto &map = g_GameEngineInternal::Game::getMap();
    if (map.empty()) return;

    // ~10% chance to randomly change direction each frame
    if (changeDist(rng) < 10) {
        int d = dirDist(rng);
        switch (d) {
            case 0: currentDirection_ = Direction::UP; break;
            case 1: currentDirection_ = Direction::DOWN; break;
            case 2: currentDirection_ = Direction::LEFT; break;
            case 3: currentDirection_ = Direction::RIGHT; break;
        }
    }

    // Movement speed
    const float speed = isScared_ ? 1.5f : 2.0f;

    // Calculate target position based on current direction
    auto currentPos = sprite_->getPosition();
    sf::Vector2f targetPos = currentPos;

    switch (currentDirection_) {
        case Direction::UP:
            targetPos.y -= speed;
            break;
        case Direction::DOWN:
            targetPos.y += speed;
            break;
        case Direction::LEFT:
            targetPos.x -= speed;
            break;
        case Direction::RIGHT:
            targetPos.x += speed;
            break;
    }

    // Check if the target position collides with a wall
    // We need to check multiple points because the sprite is 50x50
    bool canMove = true;

    // Check corners of the sprite at target position
    std::vector<sf::Vector2f> checkPoints = {
        {targetPos.x + 5, targetPos.y + 5},                       // Top-left (with small margin)
        {targetPos.x + blockSize - 5, targetPos.y + 5},           // Top-right
        {targetPos.x + 5, targetPos.y + blockSize - 5},           // Bottom-left
        {targetPos.x + blockSize - 5, targetPos.y + blockSize - 5}  // Bottom-right
    };

    for (const auto& point : checkPoints) {
        int gridX = static_cast<int>(point.x) / blockSize;
        int gridY = static_cast<int>(point.y) / blockSize;

        // Check bounds
        if (gridY < 0 || gridY >= static_cast<int>(map.size()) ||
                gridX < 0 || gridX >= static_cast<int>(map[gridY].size())) {
            canMove = false;
            break;
        }

        // Check if it's a wall
        if (map[gridY][gridX] == 'X') {
            canMove = false;
            break;
        }
    }

    // If we can move, update position
    if (canMove) {
        sprite_->setPosition(targetPos);
        this->x = static_cast<std::uint16_t>(targetPos.x);
        this->y = static_cast<std::uint16_t>(targetPos.y);
    } else {
        // If blocked, try a different random direction for next frame
        int d = dirDist(rng);
        switch (d) {
            case 0: currentDirection_ = Direction::UP; break;
            case 1: currentDirection_ = Direction::DOWN; break;
            case 2: currentDirection_ = Direction::LEFT; break;
            case 3: currentDirection_ = Direction::RIGHT; break;
        }
    }
}
