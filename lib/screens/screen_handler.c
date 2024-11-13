#include "screen_handler.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../menu/button.h"
#include "../setup.h"
#include "../utils.h"

static Screen active_screen = GAMEOVER;

Screen get_active_screen() { return active_screen; }

void set_active_screen(Screen new_screen, ScreenProperties* sp,
                       int window_width, int window_height) {
    free(sp->buttons);
    active_screen = new_screen;
    init_screen(new_screen, sp, window_width, window_height);
}

static void render_screen_buttons(ScreenProperties* sp,
                                  SDL_Renderer* renderer) {
    for (int i = 0; i < sp->buttons_len; i++) {
        render_button(&sp->buttons[i], renderer);
    }
}

static void init_screen(Screen screen, ScreenProperties* sp, int window_width,
                        int window_height) {
    switch (screen) {
        case GAMEOVER:
            init_gameover_screen(sp, window_width, window_height);
            break;

        default:
            break;
    }
}

void init_gameover_screen(ScreenProperties* sp, int window_width,
                          int window_height) {
    TTF_Font* font_small = get_fonts().pixeled_small;
    const int buttons_len = 2;

    sp->buttons = (Button*)malloc(sizeof(Button) * buttons_len);
    sp->buttons_len = buttons_len;

    const char* restart = "Restart";
    const char* return_to_menu = "Return to menu";

    Vector2 restart_sizes = get_text_size(font_small, restart);
    Vector2 return_to_menu_sizes = get_text_size(font_small, return_to_menu);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};

    sp->buttons[0] =
        create_new_button(window_width / 2 - restart_sizes.x / 2,
                          window_height / 2, font_small, c, restart);
    sp->buttons[1] = create_new_button(
        window_width / 2 - return_to_menu_sizes.x / 2, window_height / 2 + 100,
        font_small, c, return_to_menu);
}

void render_gameover_screen(SDL_Renderer* renderer, ScreenProperties* sp,
                            int score, int window_width, int window_height) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    render_screen_buttons(sp, renderer);

    TTF_Font* font = get_fonts().pixeled;
    // TTF_Font* font_small = get_fonts().pixeled_small;

    const char* gameover = "Game Over!";

    Vector2 gameover_sizes = get_text_size(font, gameover);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};
    render_text(renderer, font, window_width / 2 - gameover_sizes.x / 2,
                window_height / 2 - gameover_sizes.y / 2 - 100, c, gameover);
    // render_button(renderer, font_small, window_width / 2 - restart_sizes.x /
    // 2,
    //               window_height / 2, c, restart);
    // render_button(renderer, font_small,
    //               window_width / 2 - return_to_menu_sizes.x / 2,
    //               window_height / 2 + 100, c, return_to_menu);
}