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

bool g_PacmanEntityDecl::Pacman::canMoveInDirection(Direction dir) const {
    auto &map = g_GameEngineInternal::Game::getMap();
    if (map.empty()) return false;
    
    auto pos = p_Sprite->getPosition();
    
    // Get current grid position (with proper rounding)
    int gridX = static_cast<int>(pos.x + 0.5f) / blockSize;
    int gridY = static_cast<int>(pos.y + 0.5f) / blockSize;
    
    // Calculate target grid position
    int targetGridX = gridX;
    int targetGridY = gridY;
    
    switch (dir) {
        case Direction::UP:    targetGridY--; break;
        case Direction::DOWN:  targetGridY++; break;
        case Direction::LEFT:  targetGridX--; break;
        case Direction::RIGHT: targetGridX++; break;
    }
    
    // Check bounds
    if (targetGridY < 0 || targetGridY >= static_cast<int>(map.size()) ||
        targetGridX < 0 || targetGridX >= static_cast<int>(map[targetGridY].size())) {
        return false;
    }
    
    // Check if target is not a wall
    return map[targetGridY][targetGridX] != 'X';
}

void g_PacmanEntityDecl::Pacman::update(float deltaTime) {
    if (!isMoving_) {
        moveTimer_ = 0.0f;  // Reset timer when not moving
        return;
    }
    
    moveTimer_ += deltaTime;
    
    // Move when timer exceeds delay
    if (moveTimer_ >= MOVE_DELAY) {
        moveTimer_ = 0.0f;  // Reset timer
        e_MvLogic();        // Execute one discrete move
    }
}

void g_PacmanEntityDecl::Pacman::e_MvLogic() noexcept {
    if (!p_Sprite) return;
    
    auto &map = g_GameEngineInternal::Game::getMap();
    if (map.empty()) return;
    
    // Get current grid position
    auto pos = p_Sprite->getPosition();
    int gridX = static_cast<int>(pos.x) / blockSize;
    int gridY = static_cast<int>(pos.y) / blockSize;
    
    // Try to change to desired direction if possible
    if (desiredDirection_ != currentDirection_) {
        if (canMoveInDirection(desiredDirection_)) {
            currentDirection_ = desiredDirection_;
        }
    }
    
    // Calculate next grid position
    int nextGridX = gridX;
    int nextGridY = gridY;
    
    switch (currentDirection_) {
        case Direction::UP:    nextGridY--; break;
        case Direction::DOWN:  nextGridY++; break;
        case Direction::LEFT:  nextGridX--; break;
        case Direction::RIGHT: nextGridX++; break;
    }
    
    // Check if next position is valid
    if (nextGridY < 0 || nextGridY >= static_cast<int>(map.size()) ||
        nextGridX < 0 || nextGridX >= static_cast<int>(map[nextGridY].size())) {
        return;  // Can't move - out of bounds
    }
    
    if (map[nextGridY][nextGridX] == 'X') {
        return;  // Can't move - wall
    }
    
    // Valid move - update position to exact grid cell
    float newX = nextGridX * blockSize;
    float newY = nextGridY * blockSize;
    
    p_Sprite->setPosition({newX, newY});
    this->x = static_cast<std::uint16_t>(newX);
    this->y = static_cast<std::uint16_t>(newY);
}
