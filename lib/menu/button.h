#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../setup.h"
#include "../utils.h"

typedef struct {
    int x;
    int y;
    TTF_Font* font;
    SDL_Color c;
    const char* text;
    void (*onClick)(void* params);
    void* params;
} Button;

Button create_new_button(int x, int y, TTF_Font* font, SDL_Color c,
                         const char* text, void (*onClick)(void* params),
                         void* params);
void render_button(Button* this, SDL_Renderer* renderer, SDL_Cursor** cursor,
                   Cursors* cursors);
void handle_button_events(Button* button);
Vector2 get_button_size(Button* this);