#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../projectile/projectile.h"

#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 40

#define ENEMY_GRID_ROW_LENGTH 10
#define ENEMY_GRID_COLUMN_LENGTH 5
#define ENEMY_GAP_VALUE 10

#define SHOOTER_SHOOT_INVERVAL_MS 3000

typedef enum { BLOCKER, SHOOTER } EnemyType;

typedef struct {
    float x;
    float y;
    int w;
    int h;
    float xs;
    float ys;
    EnemyType type;
    int shoot_delay;
} Enemy;

void create_enemy_grid(Enemy* arr, int starting_x, int starting_y);
int get_enemy_grid_offset(int window_width, int enemy_row_count,
                          int enemy_width, int grid_gap);
void render_enemy(Enemy* enemy, SDL_Renderer* renderer);
void update_enemy(Enemy* enemy, int window_width, float delta_time);
float get_offset_over_border(Enemy* enemy, int window_width);
void enemy_hit(Enemy* enemies, int* enemies_length, int enemy_index_to_delete);
bool is_bullet_on_enemy(Bullet* bullet, Enemy* enemy);
bool should_spawn_bullet(Enemy* enemy);
int get_shoot_delay();