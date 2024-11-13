#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    int x;
    int y;
    TTF_Font* font;
    SDL_Color c;
    const char* text;
} Button;

Button create_new_button(int x, int y, TTF_Font* font, SDL_Color c,
                         const char* text);
void render_button(Button* this, SDL_Renderer* renderer);