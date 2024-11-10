#include "player.h"

#include <SDL2/SDL.h>

#include "../../hitboxes.c"
#include "../../utils.h"
#include "../projectile/projectile.c"

#define PLAYER_DEFAULT_HP 3
#define PLAYER_DEFAULT_SPEED 5

#define PLAYER_HP_DISPLAY_GAP 10

Player create_new_player(int x, int y, int w, int h) {
    Player new_player = {
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .hp = PLAYER_DEFAULT_HP,
        .max_hp = PLAYER_DEFAULT_HP,
        .s = PLAYER_DEFAULT_SPEED,
        .can_shoot = true,
        .projectile = create_new_bullet(-100, -100, 1, 1, -1, -1)};

    return new_player;
}

void render_player(Player* player, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_Rect rect = {
        .x = player->x, .y = player->y, .w = player->w, .h = player->h};

    SDL_RenderFillRect(renderer, &rect);
}

void update_player(Player* player, int screen_width, float delta_time) {
    if (inputs.move_left_press > 0) {
        player->x -= player->s * delta_time * 50;
        if (player->x < 0) {
            player->x = 0;
        }
    }

    if (inputs.move_right_press > 0) {
        player->x += player->s * delta_time * 50;

        if (player->x + player->w > screen_width) {
            player->x = screen_width - player->w;
        }
    }
}

void try_shoot_player_projectile(Player* player) {
    if (inputs.space_press <= 0 || player->can_shoot == false) return;

    Vector2 bullet_dim = get_default_bullet_size(PLAYER);

    Bullet b = create_new_bullet(player->x + player->w / 2 - bullet_dim.x / 2,
                                 player->y - bullet_dim.y, bullet_dim.x,
                                 bullet_dim.y, 0, -3);

    player->can_shoot = false;
    player->projectile = b;
}

void reset_player_shot(Player* player) { player->can_shoot = true; }

void player_hit(Player* player) { player->hp--; }

bool is_bullet_on_player(Bullet* bullet, Player* player) {
    return is_rect_on_rect(bullet->x, bullet->y, bullet->w, bullet->h,
                           player->x, player->y, player->w, player->h);
}

void player_display_hp(Player* player, SDL_Renderer* renderer, int x, int y,
                       int w) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    float one_hp_width =
        w / player->max_hp -
        ((float)(player->max_hp - 1) / player->max_hp) * PLAYER_HP_DISPLAY_GAP;

    for (int i = 0; i < player->hp; i++) {
        SDL_Rect rect = {.x = x + i * one_hp_width + i * PLAYER_HP_DISPLAY_GAP,
                         .y = y,
                         .w = one_hp_width,
                         .h = 20};

        SDL_RenderFillRect(renderer, &rect);
    }
}