#include "setup.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

Fonts get_fonts() {
    Fonts fonts = {
        .pixeled = TTF_OpenFont(
            "../static/fonts/Pixelify_Sans/PixelifySans-Medium.ttf", 64),
        .pixeled_small = TTF_OpenFont(
            "../static/fonts/Pixelify_Sans/PixelifySans-Medium.ttf", 44)};

    return fonts;
}

Cursors get_cursors() {
    Cursors cursors = {
        .def = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW),
        .pointer = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND)};

    return cursors;
}

Images get_images(SDL_Renderer* renderer) {
    Images images = {
        .title = IMG_LoadTexture(renderer, "../static/imgs/space-invaders.png"),
        .player_ship = IMG_LoadTexture(renderer, "../static/imgs/ship.png")};

    return images;
}