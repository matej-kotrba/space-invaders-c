#include "screen_handler.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../menu/button.h"
#include "../setup.h"
#include "../utils.h"

static Screen active_screen = GAME;

Screen get_active_screen() { return active_screen; }

void set_active_screen(Screen new_screen) { active_screen = new_screen; }

void render_gameover_screen(SDL_Renderer* renderer, int score, int window_width,
                            int window_height) {
    TTF_Font* font = get_fonts().pixeled;
    TTF_Font* font_small = get_fonts().pixeled_small;

    const char* gameover = "Game Over!";

    Vector2 sizes = get_text_size(font, gameover);
    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};
    render_text(renderer, font, window_width / 2 - sizes.x / 2,
                window_height / 2 - sizes.y / 2, c, gameover);
}