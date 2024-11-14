#include "button.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "../screens/screen_handler.h"
#include "../setup.h"
#include "../utils.h"

Button create_new_button(int x, int y, TTF_Font* font, SDL_Color c,
                         const char* text) {
    Button b = {.x = x, .y = y, .font = font, .c = c, .text = text};
    return b;
}

void render_button(Button* this, SDL_Renderer* renderer, SDL_Cursor** cursor,
                   Cursors* cursors) {
    const int BORDER_MARGIN = 15;
    int text_w, text_h;

    TTF_SizeText(this->font, this->text, &text_w, &text_h);

    SDL_Rect border_rect = {.x = this->x - BORDER_MARGIN,
                            .y = this->y - BORDER_MARGIN,
                            .w = text_w + 2 * BORDER_MARGIN,
                            .h = text_h + 2 * BORDER_MARGIN};

    SDL_SetRenderDrawColor(renderer, this->c.r, this->c.g, this->c.b,
                           this->c.a);

    if (is_point_over_rect(&inputs.mouse_pos, &border_rect)) {
        SDL_RenderDrawRect(renderer, &border_rect);
        *cursor = cursors->pointer;
    }

    SDL_Rect text_rect = {.x = this->x, .y = this->y, .w = text_w, .h = text_h};

    sdl_draw_text(renderer, this->font, this->c, text_rect, this->text);
}

// bool was_button_clicked(Button* this, Vector2 mouse_position) {
//     return is_point_over_rect(&inputs.mouse_pos, &border_rect)
// }