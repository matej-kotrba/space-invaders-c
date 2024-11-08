#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../projectile/projectile.h"

typedef struct {
    int x;
    int y;
    int w;
    int h;
    int hp;
    int max_hp;
    int s;
    bool can_shoot;
    Bullet projectile;
} Player;

Player create_new_player(int x, int y, int w, int h);

void render_player(Player*, SDL_Renderer*);
void update_player(Player* player, int screen_width);
void try_shoot_player_projectile(Player* player);
void reset_player_shot(Player* player);
void player_hit(Player* player);
bool is_bullet_on_player(Bullet* bullet, Player* player);
void player_display_hp(Player* player, SDL_Renderer* renderer, int x, int y,
                       int w);