#include "enemy.h"

#include <SDL2/SDL.h>

#include "../../hitboxes.h"
#include "../projectile/projectile.h"

#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 40

#define ENEMY_GRID_ROW_LENGTH 10
#define ENEMY_GRID_COLUMN_LENGTH 5
#define ENEMY_GAP_VALUE 10

#define SHOOTER_SHOOT_INVERVAL_MS 3000

Enemy create_new_enemy(int x, int y, float xs, float ys, EnemyType enemy_type) {
    int shoot_delay = 0;
    if (enemy_type == SHOOTER) {
        shoot_delay = get_shoot_delay();
    }

    Enemy en = {.x = x,
                .y = y,
                .xs = xs,
                .ys = ys,
                .w = ENEMY_WIDTH,
                .h = ENEMY_HEIGHT,
                .type = enemy_type,
                .shoot_delay = shoot_delay};
    return en;
}

void create_enemy_grid(Enemy* arr, int starting_x, int starting_y) {
    for (int i = 0; i < ENEMY_GRID_COLUMN_LENGTH; i++) {
        for (int j = 0; j < ENEMY_GRID_ROW_LENGTH; j++) {
            Enemy en = {.x = starting_x + (i * ENEMY_WIDTH * ENEMY_GAP_VALUE),
                        .y = starting_y};
            arr[i * ENEMY_GRID_ROW_LENGTH + j] = create_new_enemy(
                starting_x + j * ENEMY_WIDTH + j * ENEMY_GAP_VALUE,
                starting_y + i * ENEMY_HEIGHT + i * ENEMY_GAP_VALUE, 0.5, 0,
                i == 0 ? SHOOTER : BLOCKER);
        }
    }
}

int get_enemy_grid_offset(int window_width, int enemy_row_count,
                          int enemy_width, int grid_gap) {
    return (window_width - (enemy_row_count * enemy_width +
                            (enemy_row_count - 1) * grid_gap)) /
           2;
}

void render_enemy(Enemy* enemy, SDL_Renderer* renderer) {
    if (enemy->type == BLOCKER) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else if (enemy->type == SHOOTER) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    }

    SDL_Rect rect = {
        .x = enemy->x, .y = enemy->y, .w = enemy->w, .h = enemy->h};

    SDL_RenderFillRect(renderer, &rect);
}

void update_enemy(Enemy* enemy, int window_width, float delta_time) {
    enemy->x += enemy->xs * delta_time * 100;
    enemy->y += enemy->ys * delta_time * 100;

    if (enemy->x < 0)
        enemy->x = 0;
    else if (enemy->x > window_width)
        enemy->x = window_width;

    enemy->shoot_delay -= delta_time * 1000;
}

bool did_hit_wall(Enemy* enemy, int window_width) {
    return enemy->x <= 0 || enemy->x + enemy->w >= window_width;
}

void enemy_hit(Enemy* enemies, int* enemies_length, int enemy_index_to_delete) {
    for (int i = enemy_index_to_delete; i < *enemies_length - 1; i++) {
        enemies[i] = enemies[i + 1];
    }

    *enemies_length = *enemies_length - 1;
}

bool is_bullet_on_enemy(Bullet* bullet, Enemy* enemy) {
    return is_rect_on_rect(bullet->x, bullet->y, bullet->w, bullet->h, enemy->x,
                           enemy->y, enemy->w, enemy->h);
}

bool should_spawn_bullet(Enemy* enemy) {
    return enemy->type == SHOOTER && enemy->shoot_delay < 0;
}

int get_shoot_delay() {
    int r = rand() % 8000;
    return SHOOTER_SHOOT_INVERVAL_MS + r;
}