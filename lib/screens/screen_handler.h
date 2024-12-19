#pragma once

#include <SDL2/SDL.h>

#include "../game/effects/effects.h"
#include "../game/enemy/enemy.h"
#include "../game/platforms/platforms.h"
#include "../game/player/player.h"
#include "../menu/button.h"
#include "../modifiers.h"
#include "../setup.h"
#include "../utils.h"

#define ENEMY_BULLET_ALLOC_COUNT 10
#define SPREAD_EFFECTS_ALLOC_COUNT 5

#define ENEMY_SCORE 10

#define SCOREBOARD_RECORDS_ROW_LENGTH

typedef enum { MENU, GAME, GAMEOVER, SCOREBOARD, OPTIONS } Screen;

typedef struct {
    int score;
    float seconds;
} ScoreboardRecord;

typedef struct {
    SDL_Window* window;
    Fonts* fonts;
    Cursors* cursors;
    Images* images;
    Sprite* invader_sprites;
    Button* buttons;
    int buttons_len;

    SDL_Cursor* cursor;

    Modifiers modifiers;

    ScoreboardRecord* scoreboard_records;
    int scoreboard_records_len;
    int current_page;
    int max_pages;
} ScreenProperties;

typedef struct {
    int score;
    float seconds;
    Player player;
    Enemy enemies[ENEMY_GRID_ROW_LENGTH * ENEMY_GRID_COLUMN_LENGTH];
    int enemies_length;
    bool can_player_shoot;
    int enemy_bullets_length;
    int enemy_bullets_max;
    Bullet* enemy_bullets;
    int spread_effects_length;
    int spread_effects_max;
    SpreadEffect* spread_effects;
    Platform platforms[PLATFORMS_COUNT];
} GameProperties;

typedef struct {
    GameProperties* gp;
    ScreenProperties* sp;
} GameParams;

Screen get_active_screen();
void set_active_screen(Screen new_screen, GameParams* params);
void handle_screen_buttons(ScreenProperties* sp);
void render_screen_buttons(ScreenProperties* sp, SDL_Renderer* renderer);
void init_screen(Screen screen, GameParams* params);
void restart_game_fn(void* params);
void return_to_menu_fn(void* params);
void init_gameover_screen(GameParams* params);
void render_gameover_screen(SDL_Renderer* renderer, ScreenProperties* sp,
                            int score, float seconds);
void render_scoreboard_screen(SDL_Renderer* renderer, ScreenProperties* sp);
void render_menu_screen(SDL_Renderer* renderer, ScreenProperties* sp);
void render_options_screen(SDL_Renderer* renderer, ScreenProperties* sp);
void play_game_fn(void* p);
void options_fn(void* p);
void init_menu_screen(GameParams* params);
void back_fn(void* p);
void init_scoreboard_screen(GameParams* params);
void init_options_screen(GameParams* params);
void increment_page(void* p);
void decrement_page(void* p);