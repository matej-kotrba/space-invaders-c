#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../../setup.h"
#include "../projectile/projectile.h"

#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 40
#define ENEMY_SPRITE_MAX_WIDTH 24

#define ENEMY_GRID_ROW_LENGTH 10
#define ENEMY_GRID_COLUMN_LENGTH 5
#define ENEMY_GAP_VALUE 10

#define SHOOTER_SHOOT_INVERVAL_MS 3000

#define ENEMY_DEFAULT_SPRITE_CHANGE_DELAY_MS 1000.0

typedef enum { BLOCKER_A, BLOCKER_B, SHOOTER } EnemyType;

typedef struct {
    float x;
    float y;
    int w;
    int h;
    float xs;
    float ys;
    EnemyType type;
    int shoot_delay;
    Sprite* sprite;
    int sprite_index;
    float sprite_change_delay;
} Enemy;

Enemy create_new_enemy(int x, int y, float xs, float ys, EnemyType enemy_type,
                       Sprite* sprite);
void create_enemy_grid(Enemy* arr, int starting_x, int starting_y,
                       Sprite* sprites);
int get_enemy_grid_offset(int window_width, int enemy_row_count,
                          int enemy_width, int grid_gap);
void render_enemy(Enemy* enemy, SDL_Renderer* renderer);
void update_enemy(Enemy* enemy, float delta_time);
float get_offset_over_border(Enemy* enemy, int window_width);
void enemy_hit(Enemy* enemies, int* enemies_length, int enemy_index_to_delete);
bool is_bullet_on_enemy(Bullet* bullet, Enemy* enemy);
bool should_spawn_bullet(Enemy* enemy);
int get_shoot_delay();