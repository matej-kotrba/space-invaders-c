#pragma once

#include <SDL2/SDL.h>

typedef enum { MENU, GAME, GAMEOVER } Screen;

typedef struct {
    Button* buttons;
    int buttons_len;
    int buttons_max;
} ScreenProperties;

Screen get_active_screen();
void set_active_screen(Screen new_screen);
void render_gameover_screen(SDL_Renderer* renderer, int score, int window_width,
                            int window_height);