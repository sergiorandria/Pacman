# Pacman Game - C++ with SFML

A Pacman game implementation in C++ using SFML (Simple and Fast Multimedia Library) with sprite-based rendering from image assets.

## Features Implemented

### ✅ Completed
- **Sprite-Based Rendering**: All entities use PNG textures from `assets/` directory
- **Wall Rendering**: Walls rendered using wall.png texture
- **Pacman Entity**: Four directional sprites (Up, Down, Left, Right)
- **Ghost Entities**: Four colored ghosts with individual sprites
  - Red Ghost (redGhost.png)
  - Pink Ghost (pinkGhost.png)
  - Orange Ghost (orangeGhost.png)
  - Blue Ghost (blueGhost.png)
  - Scared Ghost (scaredGhost.png) - for power pellet mode
- **Food Pellets**: Small yellow dots at empty spaces
- **Power Pellets**: Large pellets at corners (uses powerFood.png or fallback)
- **Keyboard Input**: Arrow keys and WASD for direction control
- **Map Builder**: Parses map strings and creates entities
- **Graphics Renderer**: Singleton pattern renderer for all drawable entities
- **Game Loop**: Main game loop with event handling and rendering

### Assets Used

Your `assets/` directory contains:
```
assets/
├── blueGhost.png       - Blue/Cyan ghost sprite
├── cherry.png          - Bonus fruit (not yet implemented)
├── cherry2.png         - Alternative cherry sprite
├── orangeGhost.png     - Orange ghost sprite
├── pacmanDown.png      - Pacman facing down
├── pacmanLeft.png      - Pacman facing left
├── pacmanRight.png     - Pacman facing right
├── pacmanUp.png        - Pacman facing up
├── pinkGhost.png       - Pink ghost sprite
├── powerFood.png       - Power pellet sprite
├── redGhost.png        - Red ghost sprite
├── scaredGhost.png     - Frightened ghost sprite
└── wall.png            - Wall block texture
```

### Map Legend
```
X = Wall (wall.png texture)
P = Pacman (pacmanRight.png initially)
r = Red Ghost (redGhost.png)
p = Pink Ghost (pinkGhost.png)
o = Orange Ghost (orangeGhost.png)
b = Blue Ghost (blueGhost.png)
O = Exit/Special area (no pellets)
  = Empty space (food pellets rendered here)
```

## Project Structure

```
Pacman/
├── assets/              - PNG sprite images
│   ├── pacmanUp.png
│   ├── pacmanDown.png
│   ├── pacmanLeft.png
│   ├── pacmanRight.png
│   ├── redGhost.png
│   ├── pinkGhost.png
│   ├── orangeGhost.png
│   ├── blueGhost.png
│   ├── scaredGhost.png
│   ├── wall.png
│   ├── powerFood.png
│   └── cherry.png
├── e_IEntity.h          - Base entity interface
├── e_AliveEntity.h      - Base class for movable entities
├── e_Pacman.h/.cpp      - Pacman player entity with directional sprites
├── e_Ghost.h/.cpp       - Ghost enemy entities with color-specific sprites
├── e_Wall.h/.cpp        - Wall obstacles with texture
├── e_Food.h/.cpp        - Food pellets and power pellets
├── g_Constant.h         - Game constants and default map
├── g_GraphicsRenderer.h/.cpp - Rendering system
├── g_MapBuilder.h/.cpp  - Map parsing and entity creation
├── Game.h/.cpp          - Main game engine
├── main.cpp             - Entry point
└── CMakeLists.txt       - Build configuration
```

## Requirements

- C++17 or higher
- CMake 3.10 or higher
- SFML 2.5 or higher
- PNG image assets in `assets/` directory

## Building the Project

### On Linux/Mac:

```bash
# Install SFML (Ubuntu/Debian)
sudo apt-get install libsfml-dev

# Install SFML (Arch Linux)
sudo pacman -S sfml

# Install SFML (macOS with Homebrew)
brew install sfml

# Build
mkdir build
cd build
cmake ..
make

# Run (from build directory)
cd bin
./Pacman
```

### On Windows:

1. Download SFML from https://www.sfml-dev.org/download.php
2. Extract SFML to a directory (e.g., C:\SFML)
3. Set SFML_DIR environment variable or update CMakeLists.txt with path
4. Build with CMake:

```cmd
mkdir build
cd build
cmake ..
cmake --build . --config Release

# Run
cd bin\Release
Pacman.exe
```

## Controls

- **Arrow Keys** or **WASD**: Change Pacman's direction
- **ESC**: Quit game

## Current Game State

The game currently displays:
- A 21x19 Pacman map with textured walls (50x50 pixels each)
- Pacman positioned at 'P' with directional sprites
- Four ghosts at their starting positions with color-specific sprites
- Food pellets (small yellow dots) at all empty spaces
- Power pellets (large) at four corners
- Window size: 950x1050 pixels

### Sprite Loading

All entities automatically scale sprites to fit the 50x50 pixel block size:

```cpp
// Example scaling code
float scaleX = static_cast<float>(blockSize) / texture.getSize().x;
float scaleY = static_cast<float>(blockSize) / texture.getSize().y;
sprite.setScale(scaleX, scaleY);
```

### Fallback Rendering

If textures fail to load, entities fall back to colored shapes:
- **Walls**: Blue rectangles
- **Pacman**: Yellow circle
- **Ghosts**: Colored rectangles with white eyes
- **Food**: Small yellow circles
- **Power Pellets**: Large yellow circles

## TODO - Future Enhancements

### High Priority
- [ ] Implement Pacman movement (currently only direction changes)
- [ ] Collision detection with walls
- [ ] Food pellet collection
- [ ] Ghost AI movement (chase/scatter modes)
- [ ] Score system and display
- [ ] Power pellet functionality (make ghosts vulnerable)

### Medium Priority
- [ ] Lives system with display
- [ ] Level completion detection
- [ ] Ghost state management (scatter/chase/frightened)
- [ ] Bonus fruit rendering (cherry.png, cherry2.png)
- [ ] Sound effects and music
- [ ] Smooth animation between grid cells

### Low Priority
- [ ] Multiple levels with different maps
- [ ] High score tracking and persistence
- [ ] Menu system (start, pause, game over screens)
- [ ] Ghost eyes that look in movement direction
- [ ] Pacman death animation
- [ ] Victory animation

## Implementation Details

### Entity Rendering Pipeline

```
1. Load PNG texture from assets/
2. Create SFML sprite with texture
3. Scale sprite to fit 50x50 block size
4. Position sprite on grid (x * 50, y * 50)
5. Implement sf::Drawable interface
6. Draw sprite in draw() method
```

### Texture Management

Each entity class manages its own textures:

**Pacman**: 4 textures (up, down, left, right)
```cpp
sf::Texture textureUp_, textureDown_, textureLeft_, textureRight_;
```

**Ghost**: 2 textures (color-specific + scared)
```cpp
sf::Texture texture_;         // Color-specific
sf::Texture scaredTexture_;   // When vulnerable
```

**Wall**: 1 texture
```cpp
sf::Texture texture_;
```

### Direction System

Pacman uses an enum for directions:
```cpp
enum class Direction { UP, DOWN, LEFT, RIGHT };
```

Keyboard input changes direction and updates sprite:
```cpp
case sf::Keyboard::Up:
    pacman->setDirection(Direction::UP);
    // Sprite changes to pacmanUp.png
```

## Design Patterns Used

- **Singleton Pattern**: Pacman (single player) and GraphicsRenderer
- **Factory Pattern**: MapBuilder creates entities based on map characters
- **Entity-Component System**: Base Entity interface with specialized implementations
- **Strategy Pattern**: Different ghost behaviors (colors)
- **State Pattern**: Ghost states (normal/scared)

## Code Quality Improvements

1. **Texture Loading**: Robust loading with fallback to shapes
2. **Memory Management**: Used `unique_ptr` for entities
3. **Sprite Scaling**: Automatic scaling to block size
4. **Type Safety**: Enums for directions and ghost colors
5. **Const Correctness**: Drawable::draw() is const
6. **RAII**: Proper resource cleanup

## Performance Notes

- Textures are loaded once in constructor
- Sprites are reused across frames
- Only visible entities are drawn
- 60 FPS frame limit for smooth rendering
- Efficient map parsing (one-time initialization)

## Troubleshooting

### Textures Not Loading
1. Ensure `assets/` directory is in the same location as executable
2. Check file names match exactly (case-sensitive on Linux/Mac)
3. Verify PNG files are valid
4. CMake automatically copies assets to build directory

### Black Screen
- Check that textures loaded successfully
- Verify entities are being created from map
- Ensure window is not closing immediately

### Build Errors
- Verify SFML is installed correctly
- Check C++17 support is enabled
- Ensure all source files are in CMakeLists.txt

## License

This is a learning project. Feel free to use and modify as needed.

## Credits

- SFML Library: https://www.sfml-dev.org/
- Original Pacman concept: Namco
