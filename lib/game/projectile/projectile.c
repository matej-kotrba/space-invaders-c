#include "projectile.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../../hitboxes.h"
#include "../../utils.h"
#include "../enemy/enemy.h"
#include "../player/player.h"

Bullet create_new_bullet(float x, float y, int w, int h, float xs, float ys) {
    Bullet bul = {.x = x,
                  .y = y,
                  .w = w,
                  .h = h,
                  .xs = xs,
                  .ys = ys,
                  .should_delete = false};

    return bul;
}

Vector2 get_default_bullet_size(BulletType bullet_type) {
    switch (bullet_type) {
        case PLAYER:
            return new_vec2(4, 32);
            break;

        case ENEMY:
            return new_vec2(4, 12);
            break;

        default:
            return new_vec2(4, 12);
            break;
    }
}

void render_bullet(Bullet* bullet, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    SDL_Rect rect = {
        .x = bullet->x, .y = bullet->y, .w = bullet->w, .h = bullet->h};

    SDL_RenderFillRect(renderer, &rect);
}

void update_bullet(Bullet* bullet, float delta_time) {
    bullet->x += bullet->xs * delta_time * 80;
    bullet->y += bullet->ys * delta_time * 80;
}

bool should_remove_bullet(Bullet* bullet, int window_width, int window_height) {
    return bullet->x > window_width || bullet->x + bullet->w < 0 ||
           bullet->y > window_height || bullet->y + bullet->h < 0;
}