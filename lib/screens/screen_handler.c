#include "screen_handler.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../menu/button.h"
#include "../setup.h"
#include "../utils.h"

static Screen active_screen = GAMEOVER;

Screen get_active_screen() { return active_screen; }

void set_active_screen(Screen new_screen, ScreenProperties* sp, Fonts* fonts,
                       int window_width, int window_height) {
    free(sp->buttons);
    active_screen = new_screen;
    init_screen(new_screen, sp, fonts, window_width, window_height);
}

static void render_screen_buttons(ScreenProperties* sp, SDL_Renderer* renderer,
                                  Cursors* cursors) {
    for (int i = 0; i < sp->buttons_len; i++) {
        render_button(&sp->buttons[i], renderer, &sp->cursor, cursors);
    }
}

static void init_screen(Screen screen, ScreenProperties* sp, Fonts* fonts,
                        int window_width, int window_height) {
    switch (screen) {
        case GAMEOVER:
            init_gameover_screen(sp, fonts, window_width, window_height);
            break;

        default:
            break;
    }
}

void init_gameover_screen(ScreenProperties* sp, Fonts* fonts, int window_width,
                          int window_height) {
    const int buttons_len = 2;

    sp->buttons = (Button*)malloc(sizeof(Button) * buttons_len);
    sp->buttons_len = buttons_len;

    const char* restart = "Restart";
    const char* return_to_menu = "Return to menu";

    Vector2 restart_sizes = get_text_size(fonts->pixeled_small, restart);
    Vector2 return_to_menu_sizes =
        get_text_size(fonts->pixeled_small, return_to_menu);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};

    sp->buttons[0] =
        create_new_button(window_width / 2 - restart_sizes.x / 2,
                          window_height / 2, fonts->pixeled_small, c, restart);
    sp->buttons[1] = create_new_button(
        window_width / 2 - return_to_menu_sizes.x / 2, window_height / 2 + 100,
        fonts->pixeled_small, c, return_to_menu);
}

void render_gameover_screen(SDL_Renderer* renderer, ScreenProperties* sp,
                            Fonts* fonts, Cursors* cursors, int score,
                            int window_width, int window_height) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    render_screen_buttons(sp, renderer, cursors);

    const char* gameover = "Game Over!";

    Vector2 gameover_sizes = get_text_size(fonts->pixeled, gameover);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};
    render_text(renderer, fonts->pixeled,
                window_width / 2 - gameover_sizes.x / 2,
                window_height / 2 - gameover_sizes.y / 2 - 100, c, gameover);
}