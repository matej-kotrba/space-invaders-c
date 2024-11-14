#pragma once

#include <SDL2/SDL.h>

#include "../menu/button.h"
#include "../setup.h"
#include "../utils.h"

typedef enum { MENU, GAME, GAMEOVER } Screen;

typedef struct {
    Button* buttons;
    int buttons_len;

    SDL_Cursor* cursor;
} ScreenProperties;

Screen get_active_screen();

void set_active_screen(Screen new_screen, ScreenProperties* sp, Fonts* fonts,
                       int window_width, int window_height);

static void render_screen_buttons(ScreenProperties* sp, SDL_Renderer* renderer,
                                  Cursors* cursors);

static void init_screen(Screen screen, ScreenProperties* sp, Fonts* fonts,
                        int window_width, int window_height);

void init_gameover_screen(ScreenProperties* sp, Fonts* fonts, int window_width,
                          int window_height);

void render_gameover_screen(SDL_Renderer* renderer, ScreenProperties* sp,
                            Fonts* fonts, Cursors* cursors, int score,
                            int window_width, int window_height);