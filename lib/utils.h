#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    int x;
    int y;
} Vector2;

typedef struct {
    int x;
    int y;
} MousePosition;

typedef struct {
    MousePosition last_mouse_pos;
    MousePosition mouse_pos;
    float leftmouse_click;
    float move_left_press;
    float move_right_press;
    float space_press;
} Inputs;

Inputs inputs;

void update_input_attrs(Inputs* inputs);

void sdl_draw_text(SDL_Renderer*, TTF_Font*, SDL_Color, SDL_Rect, const char*);

void draw_triangle(SDL_Renderer*, int, int, int, int, int, int);
void draw_circle(SDL_Renderer* renderer, int center_x, int center_y,
                 float radius);

Vector2 new_vec2(int x, int y);

float get_random_float(float min, float max);

void remove_element(void* array, size_t element_size, int* length, int index);
void* resize_array(void* array, size_t element_size, int* max_length,
                   int alloc_count);