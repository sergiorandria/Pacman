cmake_minimum_required(VERSION 3.10)
project(Pacman)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 3.0.2 REQUIRED)

# Source files
set(SOURCES
    main.cpp
    Game.cpp
    e_Pacman.cpp
    e_Ghost.cpp
    e_Wall.cpp
    g_GraphicsRenderer.cpp
    g_MapBuilder.cpp
)

# Header files
set(HEADERS
    Game.h
    e_IEntity.h
    e_AliveEntity.h
    e_Pacman.h
    e_Ghost.h
    e_Wall.h
    g_Constant.h
    g_GraphicsRenderer.h
    g_MapBuilder.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Enable warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()
