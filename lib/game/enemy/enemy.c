#include "enemy.h"

#include <SDL2/SDL.h>

#include "../../hitboxes.h"
#include "../../setup.h"
#include "../projectile/projectile.h"

Enemy create_new_enemy(int x, int y, float xs, float ys, EnemyType enemy_type,
                       Sprite* sprite) {
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
                .shoot_delay = shoot_delay,
                .sprite = sprite};
    return en;
}

void create_enemy_grid(Enemy* arr, int starting_x, int starting_y,
                       Sprite* sprites) {
    const int template[ENEMY_GRID_ROW_LENGTH] = {SHOOTER, BLOCKER_A, BLOCKER_A,
                                                 BLOCKER_B, BLOCKER_B};

    for (int i = 0; i < ENEMY_GRID_COLUMN_LENGTH; i++) {
        for (int j = 0; j < ENEMY_GRID_ROW_LENGTH; j++) {
            Enemy en = {.x = starting_x + (i * ENEMY_WIDTH * ENEMY_GAP_VALUE),
                        .y = starting_y};
            arr[i * ENEMY_GRID_ROW_LENGTH + j] = create_new_enemy(
                starting_x + j * ENEMY_WIDTH + j * ENEMY_GAP_VALUE,
                starting_y + i * ENEMY_HEIGHT + i * ENEMY_GAP_VALUE, 1, 0,
                template[i], &(sprites[i]));
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
    if (enemy->type == BLOCKER_A) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else if (enemy->type == BLOCKER_B) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    } else if (enemy->type == SHOOTER) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    }

    SDL_Rect rect = {
        .x = enemy->x, .y = enemy->y, .w = enemy->w, .h = enemy->h};

    SDL_RenderFillRect(renderer, &rect);
}

void update_enemy(Enemy* enemy, int window_width, float delta_time) {
    enemy->x += enemy->xs * delta_time * 50.0;
    enemy->y += enemy->ys * delta_time * 50.0;

    enemy->shoot_delay -= delta_time * 1000;
}

float get_offset_over_border(Enemy* enemy, int window_width) {
    if (enemy->x < 0) {
        return enemy->x;
    } else if (enemy->x + enemy->w > (float)window_width) {
        return enemy->x + enemy->w - (float)window_width;
    } else {
        return 0.0;
    }
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