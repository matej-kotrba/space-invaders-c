#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    TTF_Font* pixeled;
    TTF_Font* pixeled_small;
    TTF_Font* pixeled_smallest;
} Fonts;

typedef struct {
    SDL_Cursor* def;
    SDL_Cursor* pointer;
} Cursors;

typedef struct {
    SDL_Texture* title;
    SDL_Texture* player_ship;
} Images;

typedef struct {
    SDL_Texture* image;
    int length;
    int width;
    int height;
} Sprite;

Fonts get_fonts();
Cursors get_cursors();
Images get_images(SDL_Renderer* renderer);
Sprite* get_invader_sprites(SDL_Renderer* renderer);