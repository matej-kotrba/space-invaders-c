cmake_minimum_required(VERSION 3.12)
project(space_invaders C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)


file(GLOB SOURCES "*.c" "lib/*.c" "lib/game/effects/*.c" "lib/game/enemy/*.c" "lib/game/platforms/*.c" "lib/game/player/*.c" "lib/game/projectile/*.c" "lib/menu/*.c" "lib/screens/*.c")

add_executable(space_invaders ${SOURCES})

find_package(SDL2 REQUIRED)

target_include_directories(space_invaders PRIVATE 
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(space_invaders SDL2 SDL2_image SDL2_ttf m)