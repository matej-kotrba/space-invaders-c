#include "input.h"

#include <string.h>

TextInput create_new_textinput(int x, int y, int w, TTF_Font* font,
                               int max_chars, void* params) {
    TextInput b = {.x = x,
                   .y = y,
                   .w = w,
                   .font = font,
                   .is_focused = false,
                   .content_max = max_chars,
                   .content = (char*)malloc(sizeof(char) * (max_chars + 1)),
                   .params = params};
    b.content[0] = '\0';
    return b;
}

void handle_keydown_textinput(TextInput* this, char pressed_key) {
    if (this->is_focused && pressed_key >= 'a' && pressed_key <= 'z' &&
        (int)strlen(this->content) < this->content_max) {
        char pressed_key_str[2] = {pressed_key, '\0'};
        strcat(this->content, pressed_key_str);
    } else if (pressed_key == SDLK_BACKSPACE && strlen(this->content) > 0) {
        this->content[strlen(this->content) - 1] = '\0';
    }
}

void handle_textinput_click(TextInput* this, Vector2* mouse_pos) {
    const char* text = "tempalte";
    int text_w, text_h;
    TTF_SizeText(this->font, text, &text_w, &text_h);

    if (is_point_over_rect(
            mouse_pos,
            &(SDL_Rect){
                .x = this->x, .y = this->y, .w = this->w, .h = text_h})) {
        this->is_focused = true;
    } else {
        this->is_focused = false;
    }
}

void render_textinput(TextInput* this, SDL_Renderer* renderer) {
    SDL_Color grayish = this->is_focused ? (SDL_Color){59, 59, 59, 255}
                                         : (SDL_Color){89, 89, 89, 255};

    SDL_SetRenderDrawColor(renderer, grayish.r, grayish.g, grayish.b,
                           grayish.a);

    int text_w, text_h;
    TTF_SizeText(this->font, this->content, &text_w, &text_h);

    SDL_Rect text_rect = {.x = this->x, .y = this->y, .w = text_w, .h = text_h};
    SDL_Rect bg_rect = {.x = this->x, .y = this->y, .w = this->w, .h = text_h};

    SDL_RenderFillRect(renderer, &bg_rect);

    sdl_draw_text(renderer, this->font, (SDL_Color){255, 255, 255, 255},
                  text_rect, this->content);
}