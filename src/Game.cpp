#include "Game.h"
#include "e_Wall.h"
#include "e_Ghost.h"
#include "e_Pacman.h"
#include "e_Food.h"
#include "g_GraphicsRenderer.h"
#include "g_MapBuilder.h"
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdlib>
#include <memory>
#include <iostream> 

g_GameEngineInternal::Map g_GameEngineInternal::Game::map;
std::shared_mutex g_GameEngineInternal::Game::m_WindowMutexLock;

g_GameEngineInternal::Game::Game() 
    : m_pacman(nullptr), score_(0), lives_(3), foodEaten_(0), totalFood_(0) {
        curGameState = g_GameEngineInternal::GameState::RUNNING;
        m_GraphicsRenderer =
            g_GameEngineInternal::Graphics::GraphicsRenderer::__pRenderGraphicsI();

        m_SharedWindowPtr = std::make_shared<sf::RenderWindow>();
        m_wRenderTexture = std::make_unique<sf::RenderTexture>();

        if (!m_SharedWindowPtr || !m_wRenderTexture) {
            std::abort();
        }

        window =
            new sf::RenderWindow(sf::VideoMode({winsize.x, winsize.y}), "Pacman");
        if (window == nullptr) {
            std::abort();
        }

        window->setFramerateLimit(60);

        // Lazy init the map and create all entity objects
        this->__call_RenderGraphicsOnce();
    }

g_GameEngineInternal::Game::~Game() {
    if (window != nullptr) {
        m_SharedWindowPtr = nullptr;
        window->close();
        delete window;
        window = nullptr;
    }
}

void g_GameEngineInternal::Game::__call_RenderGraphics() {
    // Render all entities through the GraphicsRenderer
    if (!window || !m_GraphicsRenderer) return;

    // Draw all food pellets first (background layer)
    for (const auto &food : m_food) {
        if (food) {
            m_GraphicsRenderer->renderObject(*window, *food);
        }
    }

    // Draw all walls
    for (const auto &wptr : m_walls) {
        if (wptr) {
            m_GraphicsRenderer->renderObject(*window, *wptr);
        }
    }

    // Draw all ghosts
    for (const auto &ghost : m_ghosts) {
        if (ghost) {
            m_GraphicsRenderer->renderObject(*window, *ghost);
        }
    }

    // Draw Pacman on top
    if (m_pacman) {
        m_GraphicsRenderer->renderObject(*window, *m_pacman);
    }
}

// Lazy init - create all entities from the map
constexpr void g_GameEngineInternal::Game::__call_RenderGraphicsOnce() {
    // Ensure we have a default map if none provided yet
    if (Game::map.empty()) {
        this->map = m_MapBuilder.createDefaultMap();
    }

    // Build all entities from the map
    m_walls.clear();
    m_ghosts.clear();
    m_food.clear();

    for (std::size_t y = 0; y < this->map.size(); ++y) {
        for (std::size_t x = 0; x < this->map[y].size(); ++x) {
            char cellValue = map[y][x];

            switch (cellValue) {
                case PACMAN_ENT_MAP_VALUE_WALL: 
                    {
                        // Create wall
                        auto rawWall = m_MapBuilder.createWall_(static_cast<std::uint16_t>(x),
                                static_cast<std::uint16_t>(y));
                        if (rawWall) {
                            m_walls.emplace_back(rawWall);
                        }
                        break;
                    }

                case PACMAN_ENT_MAP_VALUE_PROT: 
                    {
                        // Create Pacman
                        m_pacman = m_MapBuilder.createPacman_();
                        if (m_pacman) {
                            m_pacman->setPosition(static_cast<std::uint16_t>(x), 
                                    static_cast<std::uint16_t>(y));

                            pacmanSpawnPos_ = {static_cast<unsigned int>(x), static_cast<unsigned int>(y)};
                        }
                        break;
                    }

                case PACMAN_ENT_MAP_VALUE_RG: 
                    {
                        // Create Red Ghost
                        auto ghost = m_MapBuilder.createGhost_(
                                g_PacmanEntityDecl::g_GhostProperty::GhostColor::r);
                        if (ghost) {
                            ghost->setPosition(static_cast<std::uint16_t>(x), 
                                    static_cast<std::uint16_t>(y));
                            ghostSpawnPositions_.push_back({static_cast<unsigned int>(x), static_cast<unsigned int>(y)});
                            m_ghosts.emplace_back(ghost);
                        }
                        break;
                    }

                case PACMAN_ENT_MAP_VALUE_BG: 
                    {
                        // Create Blue Ghost
                        auto ghost = m_MapBuilder.createGhost_(
                                g_PacmanEntityDecl::g_GhostProperty::GhostColor::b);
                        if (ghost) {
                            ghost->setPosition(static_cast<std::uint16_t>(x), 
                                    static_cast<std::uint16_t>(y));
                            m_ghosts.emplace_back(ghost);
                        }
                        break;
                    }

                case PACMAN_ENT_MAP_VALUE_PG: 
                    {
                        // Create Pink Ghost
                        auto ghost = m_MapBuilder.createGhost_(
                                g_PacmanEntityDecl::g_GhostProperty::GhostColor::p);
                        if (ghost) {
                            ghost->setPosition(static_cast<std::uint16_t>(x), 
                                    static_cast<std::uint16_t>(y));
                            m_ghosts.emplace_back(ghost);
                        }
                        break;
                    }

                case PACMAN_ENT_MAP_VALUE_OG: 
                    {
                        // Create Orange Ghost
                        auto ghost = m_MapBuilder.createGhost_(
                                g_PacmanEntityDecl::g_GhostProperty::GhostColor::o);
                        if (ghost) {
                            ghost->setPosition(static_cast<std::uint16_t>(x), 
                                    static_cast<std::uint16_t>(y));
                            m_ghosts.emplace_back(ghost);
                        }
                        break;
                    }

                case PACMAN_ENT_MAP_VALUE_EXIT:
                    // Exit areas - no food pellets here
                    break;

                case PACMAN_ENT_MAP_VALUE_FOOD: 
                    {
                        // Create regular food pellet at empty spaces
                        auto food = m_MapBuilder.createFood_(static_cast<std::uint16_t>(x),
                                static_cast<std::uint16_t>(y),
                                false);
                        if (food) {
                            m_food.emplace_back(food);
                        }
                        break;
                    }

                default:
                    break;
            }
        }
    }

    // Add power pellets at corners (Bruh) 
    // Top-left area
    auto powerPellet1 = m_MapBuilder.createFood_(1, 3, true);
    if (powerPellet1) m_food.emplace_back(powerPellet1);

    // Top-right area
    auto powerPellet2 = m_MapBuilder.createFood_(17, 3, true);
    if (powerPellet2) m_food.emplace_back(powerPellet2);

    // Bottom-left area
    auto powerPellet3 = m_MapBuilder.createFood_(1, 17, true);
    if (powerPellet3) m_food.emplace_back(powerPellet3);

    // Bottom-right area
    auto powerPellet4 = m_MapBuilder.createFood_(17, 17, true);
    if (powerPellet4) m_food.emplace_back(powerPellet4);

#ifdef DEBUG 
    std::cout << "Created " << m_walls.size() << " walls" << std::endl;
    std::cout << "Created " << m_food.size() << " food pellets" << std::endl;
    std::cout << "Created " << m_ghosts.size() << " ghosts" << std::endl;
    std::cout << "Pacman created: " << (m_pacman != nullptr) << std::endl;
#endif 

    totalFood_ = m_food.size(); 
}

// Return the reference to the map object
g_GameEngineInternal::Map &g_GameEngineInternal::Game::getMap() { 
    return map; 
}

void g_GameEngineInternal::Game::respawnPacman() {
    if (m_pacman) {
        m_pacman->setPosition(pacmanSpawnPos_.x, pacmanSpawnPos_.y);
        m_pacman->setDirection(g_PacmanEntityDecl::Direction::RIGHT);
        m_pacman->setDesiredDirection(g_PacmanEntityDecl::Direction::RIGHT);
        std::cout << "Pacman respawned at (" << pacmanSpawnPos_.x << ", " << pacmanSpawnPos_.y << ")" << std::endl;
    }
}

void g_GameEngineInternal::Game::respawnGhosts() {
    for (size_t i = 0; i < m_ghosts.size() && i < ghostSpawnPositions_.size(); ++i) {
        if (m_ghosts[i]) {
            m_ghosts[i]->setPosition(ghostSpawnPositions_[i].x, ghostSpawnPositions_[i].y);
            m_ghosts[i]->setScared(false);
        }
    }
}

void g_GameEngineInternal::Game::checkFoodCollision() {
    if (!m_pacman) return;

    int pacmanGridX = m_pacman->getGridX();
    int pacmanGridY = m_pacman->getGridY();

    for (auto& food : m_food) {
        if (!food || food->isEaten()) continue;

        // Get food grid position
        int foodGridX = food->getGridX();
        int foodGridY = food->getGridY();

        // Check if Pacman is on same grid cell as food
        if (pacmanGridX == foodGridX && pacmanGridY == foodGridY) {
            food->setEaten(true);
            foodEaten_++;

            // Add score
            if (food->isPowerPellet()) {
                score_ += 50;  // Power pellet worth more
                scaredTimer_ = SCARED_DURATION;

                // Make ghosts scared
                for (auto& ghost : m_ghosts) {
                    if (ghost) {
                        //ghost->setScared(true);
                    }
                }

                // TODO: Start timer to un-scare ghosts after ~10 seconds
            } else {
                score_ += 10;  // Regular food
            }
#ifdef DEBUG 
            std::cout << "Food eaten! Score: " << score_ 
                << " (" << foodEaten_ << "/" << totalFood_ << ")" << std::endl;
#endif

        }
    }
}

void g_GameEngineInternal::Game::checkGhostCollision() {
    if (!m_pacman) return;

    int pacmanGridX = m_pacman->getGridX();
    int pacmanGridY = m_pacman->getGridY();

    for (auto& ghost : m_ghosts) {
        if (!ghost) continue;

        int ghostGridX = ghost->getGridX();
        int ghostGridY = ghost->getGridY();

        // Check if Pacman is on same grid cell as ghost
        if (pacmanGridX == ghostGridX && pacmanGridY == ghostGridY) {
            if (ghost->isScared()) {
                // Eat the ghost - send it back to spawn
                score_ += 200;
                ghost->setScared(false);
                ghost->respawn();  // Send back to starting position
                std::cout << "Ghost eaten! Score: " << score_ << std::endl;
            } else {
                // Ghost catches Pacman - lose a life
                lives_--;
                std::cout << "Caught by ghost! Lives: " << lives_ << std::endl;

                if (lives_ <= 0) {
                    // Game over
                    curGameState = GameState::PAUSED;
                    std::cout << "GAME OVER! Final Score: " << score_ << std::endl;
                } else {
                    // Respawn Pacman
                    respawnPacman();
                }
            }
        }
    }
}

void g_GameEngineInternal::Game::loop() {
    sf::Clock clock;

    while (window && window->isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();  // Convert to seconds

        // Event handling for window close only
        while (auto event = window->pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window->close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window->close();
                }
            }
        }

        // Input handling - set desired direction
        if (m_pacman) {
            bool isMoving = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
                m_pacman->setDesiredDirection(g_PacmanEntityDecl::Direction::UP);
                m_pacman->setDirection(g_PacmanEntityDecl::Direction::UP);
                isMoving = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                m_pacman->setDesiredDirection(g_PacmanEntityDecl::Direction::DOWN);
                m_pacman->setDirection(g_PacmanEntityDecl::Direction::DOWN);
                isMoving = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                m_pacman->setDesiredDirection(g_PacmanEntityDecl::Direction::LEFT);
                m_pacman->setDirection(g_PacmanEntityDecl::Direction::LEFT);
                isMoving = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || 
                    sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                m_pacman->setDesiredDirection(g_PacmanEntityDecl::Direction::RIGHT);
                m_pacman->setDirection(g_PacmanEntityDecl::Direction::RIGHT);
                isMoving = true;
            }

            m_pacman->setMoving(isMoving);
        }

        // Update game logic with delta time
        if (curGameState == GameState::RUNNING) {
            if (m_pacman) {
                m_pacman->update(dt);  // Timer-based discrete movement

                checkFoodCollision();
                checkGhostCollision(); 
            }

            // Update ghosts
            for (auto &ghost : m_ghosts) {
                if (ghost) {
                    ghost->e_MvLogic();
                }
            }
        }

        // Update scared timer
        if (scaredTimer_ > 0.0f) {
            scaredTimer_ -= dt;
            if (scaredTimer_ <= 0.0f) {
                // Time's up - ghosts back to normal
                for (auto& ghost : m_ghosts) {
                    if (ghost) {
                        ghost->setScared(false);
                    }
                }
            }
        }
        // Render
        window->clear(sf::Color::Black);
        this->__call_RenderGraphics();
        window->display();
    }
}
