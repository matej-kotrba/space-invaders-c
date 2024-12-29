#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "../hitboxes.h"
#include "../utils.h"

typedef struct {
    int x;
    int y;
    int w;
    TTF_Font* font;
    char* content;
    int content_max;
    bool is_focused;
    void* params;
} TextInput;

TextInput create_new_textinput(int x, int y, int w, TTF_Font* font,
                               int max_chars, void* params);
void handle_keydown_textinput(TextInput*, char pressed_key);
void render_textinput(TextInput*, SDL_Renderer* renderer);
void handle_textinput_click(TextInput* this, Vector2* mouse_pos);