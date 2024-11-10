#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void render_button(SDL_Renderer* renderer, TTF_Font* font, int x, int y,
                   SDL_Color c, const char* text);