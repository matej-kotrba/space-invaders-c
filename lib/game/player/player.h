#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "../projectile/projectile.h"

#define PLAYER_WIDTH 80
#define PLAYER_HEIGHT 50
#define PLAYER_DEFAULT_HP 1
#define PLAYER_DEFAULT_SPEED 200.0

#define PLAYER_HP_DISPLAY_GAP 10

typedef struct {
    float x;
    float y;
    int w;
    int h;
    int hp;
    int max_hp;
    float s;
    bool can_shoot;
    Bullet projectile;
    SDL_Texture* texture;
} Player;

Player create_new_player(float x, float y, int w, int h, SDL_Texture* texture);

void render_player(Player*, SDL_Renderer*);
void update_player(Player* player, int screen_width, float delta_time);
void try_shoot_player_projectile(Player* player);
void reset_player_shot(Player* player);
void player_hit(Player* player);
bool is_bullet_on_player(Bullet* bullet, Player* player);
void player_display_hp(Player* player, SDL_Renderer* renderer, int x, int y,
                       int w);