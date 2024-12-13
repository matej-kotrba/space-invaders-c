#include "screen_handler.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

static Screen active_screen = GAMEOVER;

Screen get_active_screen() { return active_screen; }

void set_active_screen(Screen new_screen, GameParams* params) {
    if (params->sp->buttons_len > 0) {
        free(params->sp->buttons);
    }
    params->sp->buttons_len = 0;
    active_screen = new_screen;
    init_screen(new_screen, params);
}

void handle_screen_buttons(ScreenProperties* sp) {
    for (int i = 0; i < sp->buttons_len; i++) {
        handle_button_events(&sp->buttons[i]);
    }
}

void render_screen_buttons(ScreenProperties* sp, SDL_Renderer* renderer) {
    for (int i = 0; i < sp->buttons_len; i++) {
        render_button(&sp->buttons[i], renderer, &sp->cursor, sp->cursors);
    }
}

void init_screen(Screen screen, GameParams* params) {
    switch (screen) {
        case GAMEOVER:
            init_gameover_screen(params);
            break;
        case MENU:
            init_menu_screen(params);
            break;
        case GAME:
            params->gp->score = 0;
            int window_w, window_h;
            SDL_GetWindowSize(params->sp->window, &window_w, &window_h);

            params->gp->player = create_new_player(
                (float)(window_w / 2 - 25), (float)(window_h - 100), 50, 50);

            params->gp->enemies_length =
                ENEMY_GRID_ROW_LENGTH * ENEMY_GRID_COLUMN_LENGTH;
            // Enemy enemies[ENEMY_GRID_ROW_LENGTH * ENEMY_GRID_COLUMN_LENGTH];
            create_enemy_grid(
                params->gp->enemies,
                get_enemy_grid_offset(window_h, ENEMY_GRID_ROW_LENGTH,
                                      ENEMY_WIDTH, ENEMY_GAP_VALUE),
                50);

            params->gp->can_player_shoot = true;

            params->gp->enemy_bullets_length = 0;
            params->gp->enemy_bullets_max = ENEMY_BULLET_ALLOC_COUNT;
            params->gp->enemy_bullets =
                (Bullet*)malloc(sizeof(Bullet) * ENEMY_BULLET_ALLOC_COUNT);

            params->gp->spread_effects_length = 0;
            params->gp->spread_effects_max = SPREAD_EFFECTS_ALLOC_COUNT;
            params->gp->spread_effects = (SpreadEffect*)malloc(
                sizeof(SpreadEffect) * SPREAD_EFFECTS_ALLOC_COUNT);

            const int platform_width =
                PLATFORM_PART_SIZE * PLATFORM_TEMPLATE_COLUMNS;

            for (int i = 0; i < PLATFORMS_COUNT; i++) {
                params->gp->platforms[i] = create_new_platform(
                    (window_w / PLATFORMS_COUNT) * i + platform_width, 500);
            }

            // params->gp->platforms[1] = create_new_platform(300, 500);
            // params->gp->platforms[2] = create_new_platform(500, 500);

            break;
        default:
            break;
    }
}

void restart_game_fn(void* p) {
    GameParams* params = (GameParams*)p;

    int window_w, window_h;
    SDL_GetWindowSize(params->sp->window, &window_w, &window_h);

    for (int i = 0; i < PLATFORMS_COUNT; i++) {
        free(params->gp->platforms[i].parts);
    }
    free(params->gp->enemy_bullets);
    for (int i = 0; i < params->gp->spread_effects_length; i++) {
        free(params->gp->spread_effects[i].particles);
    }
    free(params->gp->spread_effects);

    params->gp->score = 0;

    params->gp->player = create_new_player((float)(window_w / 2 - 25),
                                           (float)(window_h - 100), 50, 50);

    params->gp->enemies_length =
        ENEMY_GRID_ROW_LENGTH * ENEMY_GRID_COLUMN_LENGTH;
    Enemy enemies[ENEMY_GRID_ROW_LENGTH * ENEMY_GRID_COLUMN_LENGTH];
    create_enemy_grid(enemies,
                      get_enemy_grid_offset(window_w, ENEMY_GRID_ROW_LENGTH,
                                            ENEMY_WIDTH, ENEMY_GAP_VALUE),
                      50);

    params->gp->can_player_shoot = true;

    params->gp->enemy_bullets_length = 0;
    params->gp->enemy_bullets_max = ENEMY_BULLET_ALLOC_COUNT;

    params->gp->enemy_bullets =
        (Bullet*)malloc(sizeof(Bullet) * ENEMY_BULLET_ALLOC_COUNT);

    params->gp->spread_effects_length = 0;
    params->gp->spread_effects_max = SPREAD_EFFECTS_ALLOC_COUNT;
    params->gp->spread_effects = (SpreadEffect*)malloc(
        sizeof(SpreadEffect) * SPREAD_EFFECTS_ALLOC_COUNT);

    params->gp->platforms[0] = create_new_platform(100, 500);
    params->gp->platforms[1] = create_new_platform(300, 500);
    params->gp->platforms[2] = create_new_platform(500, 500);

    set_active_screen(GAME, params);
};

void return_to_menu_fn(void* p) {
    GameParams* params = (GameParams*)p;
    printf("Returning to menu\n");
    set_active_screen(MENU, params);
}

void init_gameover_screen(GameParams* params) {
    const int buttons_len = 2;
    int window_w, window_h;
    SDL_GetWindowSize(params->sp->window, &window_w, &window_h);

    params->sp->buttons = (Button*)malloc(sizeof(Button) * buttons_len);
    params->sp->buttons_len = buttons_len;

    const char* restart = "Restart";
    const char* return_to_menu = "Return to menu";

    Vector2 restart_sizes =
        get_text_size(params->sp->fonts->pixeled_small, restart);
    Vector2 return_to_menu_sizes =
        get_text_size(params->sp->fonts->pixeled_small, return_to_menu);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};

    params->sp->buttons[0] = create_new_button(
        window_w / 2 - restart_sizes.x / 2, window_h / 2,
        params->sp->fonts->pixeled_small, c, restart, restart_game_fn, params);
    params->sp->buttons[1] =
        create_new_button(window_w / 2 - return_to_menu_sizes.x / 2,
                          window_h / 2 + 100, params->sp->fonts->pixeled_small,
                          c, return_to_menu, return_to_menu_fn, params);
}

void play_game_fn(void* p) {
    GameParams* params = (GameParams*)p;
    restart_game_fn(params);
}

void options_fn(void* p) {
    GameParams* params = (GameParams*)p;
    printf("Options\n");
}

void init_menu_screen(GameParams* params) {
    const int buttons_len = 2;
    int window_w, window_h;
    SDL_GetWindowSize(params->sp->window, &window_w, &window_h);

    params->sp->buttons = (Button*)malloc(sizeof(Button) * buttons_len);
    params->sp->buttons_len = buttons_len;

    const char* play = "Play";
    const char* options = "Options";

    Vector2 play_sizes = get_text_size(params->sp->fonts->pixeled_small, play);
    Vector2 options_sizes =
        get_text_size(params->sp->fonts->pixeled_small, options);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};

    params->sp->buttons[0] = create_new_button(
        window_w / 2 - play_sizes.x / 2, window_h / 2,
        params->sp->fonts->pixeled_small, c, play, play_game_fn, params);
    params->sp->buttons[1] = create_new_button(
        window_w / 2 - options_sizes.x / 2, window_h / 2 + 100,
        params->sp->fonts->pixeled_small, c, options, options_fn, params);
}

void render_gameover_screen(SDL_Renderer* renderer, ScreenProperties* sp,
                            int score) {
    int window_w, window_h;
    SDL_GetWindowSize(sp->window, &window_w, &window_h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    handle_screen_buttons(sp);
    render_screen_buttons(sp, renderer);

    const char* gameover = "Game Over!";

    Vector2 gameover_sizes = get_text_size(sp->fonts->pixeled, gameover);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};
    render_text(renderer, sp->fonts->pixeled,
                window_w / 2 - gameover_sizes.x / 2,
                window_h / 2 - gameover_sizes.y / 2 - 100, c, gameover);
}

void render_menu_screen(SDL_Renderer* renderer, ScreenProperties* sp) {
    int window_w, window_h;
    SDL_GetWindowSize(sp->window, &window_w, &window_h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    handle_screen_buttons(sp);
    render_screen_buttons(sp, renderer);
}