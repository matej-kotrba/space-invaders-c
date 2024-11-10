#include "utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdlib.h>

Inputs inputs = {.last_mouse_pos = {.x = -1, .y = -1},
                 .leftmouse_click = 0,
                 .mouse_pos = {.x = -1, .y = -1}};

void update_input_attrs(Inputs* inputs) {
    inputs->leftmouse_click += 1 * inputs->leftmouse_click;
    inputs->move_left_press += 1 * inputs->move_left_press;
    inputs->move_right_press += 1 * inputs->move_right_press;
}

Vector2 new_vec2(int x, int y) {
    Vector2 vec = {.x = x, .y = y};

    return vec;
}

void sdl_draw_text(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color,
                   SDL_Rect location, const char* text) {
    // Vykreslení textu se zadaným fontem a barvou do obrázku (surface)
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    // Převod surface na hardwarovou texturu
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Vykreslení obrázku
    SDL_RenderCopy(renderer, texture, NULL, &location);

    // Uvolnění textury a surface
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void draw_triangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2,
                   int x3, int y3) {
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x3, y3, x1, y1);
}

void draw_circle(SDL_Renderer* renderer, int center_x, int center_y,
                 float radius) {
    for (int i = 0; i < radius; i++) {
        for (int j = 0; j < radius; j++) {
            int x = abs(j - radius);
            int y = abs(i - radius);
            float c = sqrt(pow(x, 2) + pow(y, 2));

            if (c <= (float)radius) {
                int start_x = center_x - (radius - j);
                int start_y = center_y - (radius);

                SDL_RenderDrawLine(renderer, start_x, start_y + i,
                                   start_x + (radius - j) * 2 + 1, start_y + i);
                SDL_RenderDrawLine(renderer, start_x, start_y + 2 * radius - i,
                                   start_x + (radius - j) * 2 + 1,
                                   start_y + 2 * radius - i);

                break;
            }
        }
    }

    // And fill the middle
    SDL_RenderDrawLine(renderer, center_x - radius + 1, center_y,
                       center_x + radius, center_y);
}

Vector2 get_text_size(TTF_Font* font, const char* text) {
    int text_w, text_h;

    TTF_SizeText(font, text, &text_w, &text_h);

    return new_vec2(text_w, text_h);
}

void render_text(SDL_Renderer* renderer, TTF_Font* font, int x, int y,
                 SDL_Color c, const char* text) {
    Vector2 sizes = get_text_size(font, text);

    SDL_Rect border_rect = {.x = x, .y = y, .w = sizes.x, .h = sizes.y};

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

    SDL_Rect text_rect = {.x = x, .y = y, .w = sizes.x, .h = sizes.y};

    sdl_draw_text(renderer, font, c, text_rect, text);
}

float get_random_float(float min, float max) {
    return min + (float)rand() / (RAND_MAX / (max - min));
}

void remove_element(void* array, size_t element_size, int* length, int index) {
    if (index < 0 || index >= *length) {
        return;
    }

    char* base = (char*)array;
    char* pos = base + index * element_size;

    memmove(pos, pos + element_size, (*length - index - 1) * element_size);

    (*length)--;
}

void* resize_array(void* array, size_t element_size, int* max_length,
                   int alloc_count) {
    *max_length += alloc_count;

    void* resized_array = realloc(array, *max_length * element_size);

    return resized_array;
}