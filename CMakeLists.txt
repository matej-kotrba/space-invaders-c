cmake_minimum_required(VERSION 3.12)
project(sdlapp C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# lib/setup.c lib/utils.c lib/hitboxes.c lib/game/effects/effects.c lib/game/enemy/enemy.c lib/game/player/player.c lib/game/projectile/projectile.c
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/main.c ${CMAKE_SOURCE_DIR}/lib/**/*.c)
# add_executable(space_invaders main.c lib/setup.c lib/utils.c lib/hitboxes.c lib/game/effects/effects.c lib/game/enemy/enemy.c lib/game/player/player.c lib/game/projectile/projectile.c)
add_executable(space_invaders ${SOURCES})

find_package(SDL2 REQUIRED)

target_include_directories(space_invaders PRIVATE 
    ${SDL2_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/lib/game
    ${CMAKE_SOURCE_DIR}/lib/game/effects
    ${CMAKE_SOURCE_DIR}/lib/game/enemy
    ${CMAKE_SOURCE_DIR}/lib/game/player
    ${CMAKE_SOURCE_DIR}/lib/game/projectile
    ${CMAKE_SOURCE_DIR}/lib/game/platforms
)
target_link_libraries(space_invaders SDL2 SDL2_image SDL2_ttf m)