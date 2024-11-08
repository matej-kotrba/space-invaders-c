cmake_minimum_required(VERSION 3.12)
project(sdlapp C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# lib/setup.c lib/utils.c lib/hitboxes.c lib/game/effects/effects.c lib/game/enemy/enemy.c lib/game/player/player.c lib/game/projectile/projectile.c
add_executable(space_invaders main.c)

find_package(SDL2 REQUIRED)

target_include_directories(space_invaders PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(space_invaders SDL2 SDL2_image SDL2_ttf m)