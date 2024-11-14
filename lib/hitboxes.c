#include <stdbool.h>

#include "utils.h"

bool is_rect_on_rect(int x1, int y1, int w1, int h1, int x2, int y2, int w2,
                     int h2) {
    return x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2;
}

bool is_point_over_rect(const Vector2* point, const SDL_Rect* rect) {
    return point->x >= rect->x && point->x <= rect->x + rect->w &&
           point->y >= rect->y && point->y <= rect->y + rect->h;
}