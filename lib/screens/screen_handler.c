#include "screen_handler.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

static Screen active_screen = MENU;

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
        case SCOREBOARD:
            init_scoreboard_screen(params);
            break;
        case OPTIONS:
            init_options_screen(params);
            break;
        case GAME:
            params->gp->score = 0;
            int window_w, window_h;
            SDL_GetWindowSize(params->sp->window, &window_w, &window_h);

            params->gp->player = create_new_player(
                (float)(window_w / 2 - 25), (float)(window_h - 100),
                PLAYER_WIDTH, PLAYER_HEIGHT, params->sp->images->player_ship);

            params->gp->enemies_length =
                ENEMY_GRID_ROW_LENGTH * ENEMY_GRID_COLUMN_LENGTH;
            // Enemy enemies[ENEMY_GRID_ROW_LENGTH * ENEMY_GRID_COLUMN_LENGTH];
            create_enemy_grid(
                params->gp->enemies,
                get_enemy_grid_offset(window_h, ENEMY_GRID_ROW_LENGTH,
                                      ENEMY_WIDTH, ENEMY_GAP_VALUE),
                50, params->sp->invader_sprites);

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

            params->gp->platforms = (Platform*)malloc(
                sizeof(Platform) *
                params->sp->modifiers.modifiers_int[PLATFORMS_COUNT].current);
            for (int i = 0;
                 i <
                 params->sp->modifiers.modifiers_int[PLATFORMS_COUNT].current;
                 i++) {
                params->gp->platforms[i] = create_new_platform(
                    (window_w /
                     params->sp->modifiers.modifiers_int[PLATFORMS_COUNT]
                         .current) *
                            i +
                        (window_w /
                         params->sp->modifiers.modifiers_int[PLATFORMS_COUNT]
                             .current /
                         2) -
                        platform_width / 2,
                    500);
            }

            break;
        default:
            break;
    }
}

void restart_game_fn(void* p) {
    GameParams* params = (GameParams*)p;

    int window_w, window_h;
    SDL_GetWindowSize(params->sp->window, &window_w, &window_h);

    if (params->gp->did_play) {
        game_cleanup(params);
    }

    // if (params->gp->platforms != NULL) {
    //     free(params->gp->platforms);
    // }
    // params->gp->platforms = (Platform*)malloc(
    //     sizeof(Platform) *
    //     params->sp->modifiers.modifiers_int[PLATFORMS_COUNT].current);
    // for (int i = 0;
    //      i < params->sp->modifiers.modifiers_int[PLATFORMS_COUNT].current;
    //      i++) {
    //     free(params->gp->platforms[i].parts);
    // }

    // free(params->gp->enemy_bullets);
    // printf("Restarting game\n");
    // for (int i = 0; i < params->gp->spread_effects_length; i++) {
    //     free(params->gp->spread_effects[i].particles);
    // }
    // free(params->gp->spread_effects);

    params->gp->score = 0;

    params->gp->player = create_new_player(
        (float)(window_w / 2 - 25), (float)(window_h - 100), PLAYER_WIDTH,
        PLAYER_HEIGHT, params->sp->images->player_ship);

    params->gp->enemies_length =
        ENEMY_GRID_ROW_LENGTH * ENEMY_GRID_COLUMN_LENGTH;
    create_enemy_grid(params->gp->enemies,
                      get_enemy_grid_offset(window_h, ENEMY_GRID_ROW_LENGTH,
                                            ENEMY_WIDTH, ENEMY_GAP_VALUE),
                      50, params->sp->invader_sprites);

    params->gp->can_player_shoot = true;

    params->gp->enemy_bullets_length = 0;
    params->gp->enemy_bullets_max = ENEMY_BULLET_ALLOC_COUNT;
    params->gp->enemy_bullets =
        (Bullet*)malloc(sizeof(Bullet) * ENEMY_BULLET_ALLOC_COUNT);

    params->gp->spread_effects_length = 0;
    params->gp->spread_effects_max = SPREAD_EFFECTS_ALLOC_COUNT;
    params->gp->spread_effects = (SpreadEffect*)malloc(
        sizeof(SpreadEffect) * SPREAD_EFFECTS_ALLOC_COUNT);

    const int platform_width = PLATFORM_PART_SIZE * PLATFORM_TEMPLATE_COLUMNS;

    params->gp->platforms = (Platform*)malloc(
        sizeof(Platform) *
        params->sp->modifiers.modifiers_int[PLATFORMS_COUNT].current);

    for (int i = 0;
         i < params->sp->modifiers.modifiers_int[PLATFORMS_COUNT].current;
         i++) {
        params->gp->platforms[i] = create_new_platform(
            (window_w /
             params->sp->modifiers.modifiers_int[PLATFORMS_COUNT].current) *
                    i +
                (window_w /
                 params->sp->modifiers.modifiers_int[PLATFORMS_COUNT].current /
                 2) -
                platform_width / 2,
            500);
    }
    // printf("Restarting game\n");
    params->gp->did_play = true;

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

void back_fn(void* p) {
    GameParams* params = (GameParams*)p;
    set_active_screen(MENU, params);
}

void increment_page(void* p) {
    GameParams* params = (GameParams*)p;
    if (params->sp->current_page < params->sp->max_pages)
        params->sp->current_page++;
}

void decrement_page(void* p) {
    GameParams* params = (GameParams*)p;
    if (params->sp->current_page > 0) params->sp->current_page--;
}

void init_scoreboard_screen(GameParams* params) {
    const int buttons_len = 3;
    int window_w, window_h;
    SDL_GetWindowSize(params->sp->window, &window_w, &window_h);

    params->sp->buttons = (Button*)malloc(sizeof(Button) * buttons_len);
    params->sp->buttons_len = buttons_len;
    params->sp->current_page = 0;

    const char* back = "<--";
    const char* page_f = ">";
    const char* page_b = "<";

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};

    params->sp->buttons[0] = create_new_button(
        20, 20, params->sp->fonts->pixeled_small, c, back, back_fn, params);
    params->sp->buttons[1] = create_new_button(
        window_w / 2 - 80, window_h - 80, params->sp->fonts->pixeled_small, c,
        page_b, decrement_page, params);
    params->sp->buttons[2] = create_new_button(
        window_w / 2 + 60, window_h - 80, params->sp->fonts->pixeled_small, c,
        page_f, increment_page, params);

    FILE* scoreboard = fopen("scoreboard.txt", "r");
    if (scoreboard == NULL) {
        return;
    }

    char buffer[100];
    int n = 0;
    while (fgets(buffer, 100, scoreboard) != NULL) {
        params->sp->scoreboard_records = realloc(
            params->sp->scoreboard_records, sizeof(ScoreboardRecord) * (++n));
        char* ptr = strtok(buffer, ";");
        int i = 0;

        while (ptr != NULL) {
            switch (i) {
                case 0:
                    params->sp->scoreboard_records[n - 1].score = atoi(ptr);
                    break;
                case 1:
                    params->sp->scoreboard_records[n - 1].seconds = atof(ptr);
                    break;
            }
            i++;
            ptr = strtok(NULL, ";");
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (params->sp->scoreboard_records[j].score <
                    params->sp->scoreboard_records[j + 1].score ||
                params->sp->scoreboard_records[j].score ==
                        params->sp->scoreboard_records[j + 1].score &&
                    params->sp->scoreboard_records[j].seconds >
                        params->sp->scoreboard_records[j + 1].seconds) {
                ScoreboardRecord temp = params->sp->scoreboard_records[j];
                params->sp->scoreboard_records[j] =
                    params->sp->scoreboard_records[j + 1];
                params->sp->scoreboard_records[j + 1] = temp;
            }
        }
    }

    params->sp->scoreboard_records_len = n;

    fclose(scoreboard);
}

void options_fn(void* p) {
    GameParams* params = (GameParams*)p;
    set_active_screen(OPTIONS, params);
}

void init_options_screen(GameParams* params) {
    const int buttons_len = 1;
    int window_w, window_h;
    SDL_GetWindowSize(params->sp->window, &window_w, &window_h);

    params->sp->buttons = (Button*)malloc(sizeof(Button) * buttons_len);
    params->sp->buttons_len = buttons_len;

    const char* back = "<--";

    Vector2 back_sizes = get_text_size(params->sp->fonts->pixeled_small, back);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};

    params->sp->buttons[0] = create_new_button(
        20, 20, params->sp->fonts->pixeled_small, c, back, back_fn, params);
}

void play_game_fn(void* p) {
    GameParams* params = (GameParams*)p;
    restart_game_fn(params);
}

void scoreboard_fn(void* p) {
    GameParams* params = (GameParams*)p;
    set_active_screen(SCOREBOARD, params);
}

void init_menu_screen(GameParams* params) {
    const int buttons_len = 3;
    int window_w, window_h;
    SDL_GetWindowSize(params->sp->window, &window_w, &window_h);

    params->sp->buttons = (Button*)malloc(sizeof(Button) * buttons_len);
    params->sp->buttons_len = buttons_len;

    const char* play = "Play";
    const char* scoreboard = "Scoreboard";
    const char* options = "Options";

    Vector2 play_sizes = get_text_size(params->sp->fonts->pixeled_small, play);
    Vector2 scoreboard_sizes =
        get_text_size(params->sp->fonts->pixeled_small, scoreboard);
    Vector2 options_sizes =
        get_text_size(params->sp->fonts->pixeled_small, options);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};

    params->sp->buttons[0] = create_new_button(
        window_w / 2 - play_sizes.x / 2, window_h / 2,
        params->sp->fonts->pixeled_small, c, play, play_game_fn, params);
    params->sp->buttons[1] = create_new_button(
        window_w / 2 - scoreboard_sizes.x / 2, window_h / 2 + 100,
        params->sp->fonts->pixeled_small, c, scoreboard, scoreboard_fn, params);
    params->sp->buttons[2] = create_new_button(
        window_w / 2 - options_sizes.x / 2, window_h / 2 + 200,
        params->sp->fonts->pixeled_small, c, options, options_fn, params);
}

void render_gameover_screen(SDL_Renderer* renderer, ScreenProperties* sp,
                            int score, float seconds) {
    int window_w, window_h;
    SDL_GetWindowSize(sp->window, &window_w, &window_h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    handle_screen_buttons(sp);
    render_screen_buttons(sp, renderer);

    const char* gameover = "Game Over!";
    char score_text[20];
    sprintf(score_text, "Score: %d", score);
    char time_text[20];

    int minutes = (int)(seconds / 60);
    int secs = (int)(seconds - (minutes * 60));
    int milisecs = (int)((seconds - (minutes * 60) - secs) * 1000);
    sprintf(time_text, "Time: %02d:%02d.%03d", minutes, secs, milisecs);

    Vector2 gameover_sizes = get_text_size(sp->fonts->pixeled, gameover);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};
    render_text(renderer, sp->fonts->pixeled,
                window_w / 2 - gameover_sizes.x / 2,
                window_h / 2 - gameover_sizes.y / 2 - 100, c, gameover);
    render_text(renderer, sp->fonts->pixeled_small, 40, 20, c, score_text);
    render_text(renderer, sp->fonts->pixeled_small, 40, 80, c, time_text);
}

void render_scoreboard_screen(SDL_Renderer* renderer, ScreenProperties* sp) {
    int window_w, window_h;
    SDL_GetWindowSize(sp->window, &window_w, &window_h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    handle_screen_buttons(sp);
    render_screen_buttons(sp, renderer);

    const int row_height = 50;

    const int cols_per_page = 2;
    const int rows_per_page = (window_h - 160) / row_height;

    sp->max_pages =
        sp->scoreboard_records_len / (rows_per_page * cols_per_page);

    for (int i = sp->current_page * rows_per_page;
         i < sp->scoreboard_records_len &&
         (sp->current_page + 1) * rows_per_page * cols_per_page;
         i++) {
        char buffer[30];

        int minutes = (int)(sp->scoreboard_records[i].seconds / 60);
        int secs = (int)(sp->scoreboard_records[i].seconds - (minutes * 60));
        int milisecs =
            (int)((sp->scoreboard_records[i].seconds - (minutes * 60) - secs) *
                  1000);
        sprintf(buffer, "%03d (%02d:%02d.%02d)",
                sp->scoreboard_records[i].score, minutes, secs, milisecs);
        render_text(
            renderer, sp->fonts->pixeled_small,
            20 + ((i - sp->current_page * rows_per_page * cols_per_page) /
                  rows_per_page) *
                     window_w / cols_per_page,
            80 + (i % rows_per_page) * row_height,
            (SDL_Color){255, 255, 255, 255}, buffer);
    }

    if (sp->scoreboard_records == NULL) {
        char* no_records = "No records yet!";
        Vector2 no_records_sizes =
            get_text_size(sp->fonts->pixeled, no_records);
        SDL_Color c = {255, 255, 255, 255};
        render_text(renderer, sp->fonts->pixeled,
                    window_w / 2 - no_records_sizes.x / 2,
                    window_h / 2 - no_records_sizes.y / 2, c, no_records);
    }

    char pages[10];
    sprintf(pages, "%d/%d", sp->current_page + 1,
            sp->scoreboard_records_len / (rows_per_page * cols_per_page) + 1);
    Vector2 pages_sizes = get_text_size(sp->fonts->pixeled_small, pages);
    SDL_Color c = {255, 255, 255, 255};
    render_text(renderer, sp->fonts->pixeled_small,
                window_w / 2 - pages_sizes.x / 2, window_h - 80, c, pages);
}

void render_menu_screen(SDL_Renderer* renderer, ScreenProperties* sp) {
    int window_w, window_h;
    SDL_GetWindowSize(sp->window, &window_w, &window_h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    handle_screen_buttons(sp);
    render_screen_buttons(sp, renderer);

    SDL_Rect rect = {.x = window_w / 2 - 250, .y = 50, .w = 500, .h = 200};

    SDL_RenderCopyEx(renderer, sp->images->title, NULL, &rect, 0, NULL,
                     SDL_FLIP_NONE);
}

void render_options_screen(SDL_Renderer* renderer, ScreenProperties* sp) {
    int window_w, window_h;
    SDL_GetWindowSize(sp->window, &window_w, &window_h);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    handle_screen_buttons(sp);
    render_screen_buttons(sp, renderer);

    const char* options = "Options";
    Vector2 options_sizes = get_text_size(sp->fonts->pixeled, options);

    SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};
    render_text(renderer, sp->fonts->pixeled,
                window_w / 2 - options_sizes.x / 2, 20, c, options);
}

void game_cleanup(GameParams* params) {
    for (int i = 0;
         i < params->sp->modifiers.modifiers_int[PLATFORMS_COUNT].current;
         i++) {
        free(params->gp->platforms[i].parts);
    }
    free(params->gp->platforms);

    free(params->gp->enemy_bullets);
    for (int i = 0; i < params->gp->spread_effects_length; i++) {
        free(params->gp->spread_effects[i].particles);
    }
    free(params->gp->spread_effects);
}