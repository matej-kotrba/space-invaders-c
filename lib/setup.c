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

Sprite* get_invader_sprites(SDL_Renderer* renderer) {
    int shooter_w, shooter_h;
    SDL_Texture* shooter =
        IMG_LoadTexture(renderer, "../static/sprites/invader-B.png");
    SDL_QueryTexture(shooter, NULL, NULL, &shooter_w, &shooter_h);

    int blocker_A_w, blocker_A_h;
    SDL_Texture* blocker_A =
        IMG_LoadTexture(renderer, "../static/sprites/invader-C.png");
    SDL_QueryTexture(blocker_A, NULL, NULL, &blocker_A_w, &blocker_A_h);

    int blocker_B_w, blocker_B_h;
    SDL_Texture* blocker_B =
        IMG_LoadTexture(renderer, "../static/sprites/invader-A.png");
    SDL_QueryTexture(blocker_B, NULL, NULL, &blocker_B_w, &blocker_B_h);

    Sprite* sprites = malloc(sizeof(Sprite) * 3);
    sprites[0] = (Sprite){.image = shooter,
                          .width = shooter_w / 2,
                          .height = shooter_h,
                          .length = 2};
    sprites[1] = (Sprite){.image = blocker_A,
                          .width = blocker_A_w / 2,
                          .height = blocker_A_h,
                          .length = 2};
    sprites[2] = (Sprite){.image = blocker_B,
                          .width = blocker_B_w / 2,
                          .height = blocker_B_h,
                          .length = 2};

    return sprites;
}