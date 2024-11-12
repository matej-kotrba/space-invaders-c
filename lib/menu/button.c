#include "button.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../setup.h"
#include "../utils.h"

void render_button(SDL_Renderer* renderer, TTF_Font* font, int x, int y,
                   SDL_Color c, const char* text) {
    const int BORDER_MARGIN = 15;
    int text_w, text_h;
    SDL_Cursor* pointer = get_cursors().pointer;

    TTF_SizeText(font, text, &text_w, &text_h);

    SDL_Rect border_rect = {.x = x - BORDER_MARGIN,
                            .y = y - BORDER_MARGIN,
                            .w = text_w + 2 * BORDER_MARGIN,
                            .h = text_h + 2 * BORDER_MARGIN};

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

    if (is_point_over_rect(&inputs.mouse_pos, &border_rect)) {
        SDL_RenderDrawRect(renderer, &border_rect);
        SDL_SetCursor(pointer);
    }

    SDL_Rect text_rect = {.x = x, .y = y, .w = text_w, .h = text_h};

    sdl_draw_text(renderer, font, c, text_rect, text);
}