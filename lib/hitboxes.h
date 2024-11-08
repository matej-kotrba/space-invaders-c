#include <SDL2/SDL.h>
#include <stdbool.h>

#include "utils.h"

bool is_rect_on_rect(int x1, int y1, int w1, int h1, int x2, int y2, int w2,
                     int h2);

bool is_point_over_rect(const MousePosition* point, const SDL_Rect* rect);