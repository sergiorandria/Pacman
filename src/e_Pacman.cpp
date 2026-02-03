#include "Game.h"
#include "e_Pacman.h"

using g_PacmanEntityDecl::Pacman;

g_PacmanEntityDecl::Pacman *g_PacmanEntityDecl::Pacman::instance = nullptr;
std::mutex g_PacmanEntityDecl::Pacman::p_MutexLock;

g_PacmanEntityDecl::Pacman *Pacman::Instance() noexcept {
    if ((Pacman *)instance == nullptr) {
        std::lock_guard<std::mutex> lock_(p_MutexLock);
        if ((Pacman *)instance == nullptr) {
            instance = new Pacman();
        }
    }

    return instance;
}

void g_PacmanEntityDecl::Pacman::e_MvLogic() noexcept {
    if (!isMoving_) return;  

    // Pacman movement with collision detection
    if (!p_Sprite) return;

    // Get the game map for collision checking
    auto &map = g_GameEngineInternal::Game::getMap();
    if (map.empty()) return;

    // Movement speed (pixels per frame at 60 FPS)
    constexpr float speed = 6.0f;

    // Calculate target position based on current direction
    auto currentPos = p_Sprite->getPosition();
    sf::Vector2f targetPos = currentPos;

    switch (currentDirection_) {
        case g_PacmanEntityDecl::Direction::UP:
            targetPos.y -= speed;
            break;
        case g_PacmanEntityDecl::Direction::DOWN:
            targetPos.y += speed;
            break;
        case g_PacmanEntityDecl::Direction::LEFT:
            targetPos.x -= speed;
            break;
        case g_PacmanEntityDecl::Direction::RIGHT:
            targetPos.x += speed;
            break;
    }

    // Check if the target position collides with a wall
    // We need to check multiple points because the sprite is 50x50
    bool canMove = true;

    // Check corners of the sprite at target position (with small margin)
    std::vector<sf::Vector2f> checkPoints = {
        {targetPos.x + 10, targetPos.y + 10},                       // Top-left
        {targetPos.x + blockSize - 10, targetPos.y + 10},           // Top-right
        {targetPos.x + 10, targetPos.y + blockSize - 10},           // Bottom-left
        {targetPos.x + blockSize - 10, targetPos.y + blockSize - 10}  // Bottom-right
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
        p_Sprite->setPosition(targetPos);
        this->x = static_cast<std::uint16_t>(targetPos.x);
        this->y = static_cast<std::uint16_t>(targetPos.y);
    }
    // If blocked, Pacman stays in place (doesn't change direction)


}
