#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

#include "lib/game/effects/effects.c"
#include "lib/game/enemy/enemy.c"
#include "lib/game/platforms/platforms.c"
#include "lib/game/player/player.c"
#include "lib/game/projectile/projectile.h"
#include "lib/menu/button.c"
#include "lib/screens/screen_handler.c"
#include "lib/setup.c"
#include "lib/utils.c"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

// Game
#define ENEMY_BULLET_ALLOC_COUNT 10
#define SPREAD_EFFECTS_ALLOC_COUNT 5

#define PLATFORMS_COUNT 3

#define ENEMY_SCORE 10

int main(int argc, char* argv[]) {
    // SDL init
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // Window init
    SDL_Window* window = SDL_CreateWindow(
        "SDL experiments",  // Titulek okna
        100,                // Souřadnice x
        100,                // Souřadnice y
        WINDOW_WIDTH,       // Šířka
        WINDOW_HEIGHT,      // Výška
        SDL_WINDOW_SHOWN    // Okno se má po vytvoření rovnou zobrazit
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    // Setup
    srand(time(NULL));
    Uint32 previous_time = SDL_GetTicks();
    float delta_time;

    Fonts fonts = get_fonts();
    Cursors cursors = get_cursors();

    ScreenProperties screen_properties;
    init_screen(get_active_screen(), &screen_properties, &fonts, WINDOW_WIDTH,
                WINDOW_HEIGHT);

    // Game objects
    int score = 0;

    Player player = create_new_player((float)(WINDOW_WIDTH / 2 - 25),
                                      (float)(WINDOW_HEIGHT - 100), 50, 50);

    int enemies_length = ENEMY_GRID_ROW_LENGTH * ENEMY_GRID_COLUMN_LENGTH;
    Enemy enemies[ENEMY_GRID_ROW_LENGTH * ENEMY_GRID_COLUMN_LENGTH];
    create_enemy_grid(enemies,
                      get_enemy_grid_offset(WINDOW_WIDTH, ENEMY_GRID_ROW_LENGTH,
                                            ENEMY_WIDTH, ENEMY_GAP_VALUE),
                      50);

    bool can_player_shoot = true;

    int enemy_bullets_length = 0;
    int enemy_bullets_max = ENEMY_BULLET_ALLOC_COUNT;
    Bullet* enemy_bullets;
    enemy_bullets = (Bullet*)malloc(sizeof(Bullet) * ENEMY_BULLET_ALLOC_COUNT);

    int spread_effects_length = 0;
    int spread_effects_max = SPREAD_EFFECTS_ALLOC_COUNT;
    SpreadEffect* spread_effects = (SpreadEffect*)malloc(
        sizeof(SpreadEffect) * SPREAD_EFFECTS_ALLOC_COUNT);

    Platform platforms[PLATFORMS_COUNT] = {create_new_platform(100, 500),
                                           create_new_platform(300, 500),
                                           create_new_platform(500, 500)};

    SDL_Event event;
    bool running = true;

    while (running == true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT ||
                    event.key.keysym.sym == SDLK_a) {
                    if (inputs.move_left_press < 1) {
                        inputs.move_left_press = 1;
                    }
                }

                if (event.key.keysym.sym == SDLK_RIGHT ||
                    event.key.keysym.sym == SDLK_d) {
                    if (inputs.move_right_press < 1) {
                        inputs.move_right_press = 1;
                    }
                }

                if (event.key.keysym.sym == SDLK_SPACE) {
                    inputs.space_press = 1;
                }
            }

            if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LEFT ||
                    event.key.keysym.sym == SDLK_a) {
                    inputs.move_left_press = -1;
                }

                if (event.key.keysym.sym == SDLK_RIGHT ||
                    event.key.keysym.sym == SDLK_d) {
                    inputs.move_right_press = -1;
                }

                if (event.key.keysym.sym == SDLK_SPACE) {
                    inputs.space_press = -1;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    inputs.leftmouse_click = 1;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    inputs.leftmouse_click = -1;
                }
            }
        }

        Uint32 current_time = SDL_GetTicks();
        delta_time = (current_time - previous_time) / 1000.0f;
        previous_time = current_time;

        int mouse_x, mouse_y;
        Uint32 mouse_buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

        inputs.mouse_pos.x = mouse_x;
        inputs.mouse_pos.y = mouse_y;

        update_input_attrs(&inputs);

        // SDL_SetCursor(cursors.def);
        // SDL_SetCursor(cursors.pointer);

        screen_properties.cursor = cursors.def;

        switch (get_active_screen()) {
            case GAME:
                // Game update cycle
                update_player(&player, WINDOW_WIDTH, delta_time);
                try_shoot_player_projectile(&player);

                if (player.can_shoot == false) {
                    update_bullet(&player.projectile, delta_time);
                }

                if (should_remove_bullet(&player.projectile, WINDOW_WIDTH,
                                         WINDOW_HEIGHT)) {
                    reset_player_shot(&player);
                }

                if (player.can_shoot == false) {
                    for (int i = 0; i < PLATFORMS_COUNT; i++) {
                        int index = get_platform_bullet_hit_part_index(
                            &platforms[i], &player.projectile);
                        if (index == -1) continue;
                        platform_hit(&platforms[i], &player.projectile, index);
                        reset_player_shot(&player);
                    }
                }

                for (int i = 0; i < enemy_bullets_length; i++) {
                    update_bullet(&enemy_bullets[i], delta_time);
                }

                for (int i = 0; i < enemy_bullets_length; i++) {
                    if (is_bullet_on_player(&enemy_bullets[i], &player)) {
                        player_hit(&player);
                        enemy_bullets[i].should_delete = true;
                        if (player.hp <= 0) {
                            set_active_screen(GAMEOVER, &screen_properties,
                                              &fonts, WINDOW_WIDTH,
                                              WINDOW_HEIGHT);
                        }
                        continue;
                    }

                    if (should_remove_bullet(&enemy_bullets[i], WINDOW_WIDTH,
                                             WINDOW_HEIGHT)) {
                        enemy_bullets[i].should_delete = true;
                        continue;
                    }

                    for (int j = 0; j < PLATFORMS_COUNT; j++) {
                        int index = get_platform_bullet_hit_part_index(
                            &platforms[j], &enemy_bullets[i]);
                        if (index == -1) continue;
                        platform_hit(&platforms[j], &enemy_bullets[i], index);
                        enemy_bullets[i].should_delete = true;
                    }
                }

                for (int i = 0; i < enemies_length; i++) {
                    update_enemy(&enemies[i], WINDOW_WIDTH, delta_time);

                    if (should_spawn_bullet(&enemies[i])) {
                        if (enemy_bullets_length == enemy_bullets_max) {
                            resize_array(enemy_bullets, sizeof(Bullet),
                                         &enemy_bullets_max,
                                         ENEMY_BULLET_ALLOC_COUNT);
                        }

                        Vector2 vec = get_default_bullet_size(ENEMY);
                        enemy_bullets[enemy_bullets_length] = create_new_bullet(
                            enemies[i].x + enemies[i].w / 2,
                            enemies[i].y + enemies[i].h, vec.x, vec.y, 0,
                            get_random_float(2, 3));

                        enemies[i].shoot_delay = get_shoot_delay();
                        enemy_bullets_length++;
                    }
                }

                if (player.can_shoot == false) {
                    for (int i = 0; i < enemies_length; i++) {
                        if (is_bullet_on_enemy(&player.projectile,
                                               &enemies[i])) {
                            if (spread_effects_length == spread_effects_max) {
                                resize_array(spread_effects,
                                             sizeof(SpreadEffect),
                                             &spread_effects_max,
                                             SPREAD_EFFECTS_ALLOC_COUNT);
                            }

                            spread_effects[spread_effects_length] =
                                create_new_spread_effect(
                                    enemies[i].x + enemies[i].w / 2,
                                    enemies[i].y + enemies[i].h / 2);

                            spread_effects_length++;

                            enemy_hit(enemies, &enemies_length, i);
                            player.can_shoot = true;
                            score += ENEMY_SCORE;

                            i--;
                        }
                    }
                }

                for (int i = 0; i < enemies_length; i++) {
                    float offset =
                        get_offset_over_border(&enemies[i], WINDOW_WIDTH);
                    if (offset != 0) {
                        for (int j = 0; j < enemies_length; j++) {
                            enemies[j].xs *= -1;
                            enemies[j].x += -offset;
                        }
                        break;
                    }
                }

                for (int i = 0; i < spread_effects_length; i++) {
                    update_spread_effect(&spread_effects[i], delta_time);
                    if (should_remove_spread_effect(&spread_effects[i])) {
                        free(spread_effects->particles);
                        remove_element(spread_effects, sizeof(SpreadEffect),
                                       &spread_effects_length, i);
                        i--;
                    }
                }

                for (int i = 0; i < enemy_bullets_length; i++) {
                    if (enemy_bullets[i].should_delete) {
                        remove_element(enemy_bullets, sizeof(Bullet),
                                       &enemy_bullets_length, i);
                    }
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                // Render effects
                for (int i = 0; i < spread_effects_length; i++) {
                    render_spread_effect(&spread_effects[i], renderer);
                }

                // Render Characters
                for (int i = 0; i < enemies_length; i++) {
                    render_enemy(&(enemies[i]), renderer);
                }

                for (int i = 0; i < enemy_bullets_length; i++) {
                    render_bullet(&enemy_bullets[i], renderer);
                }

                for (int i = 0; i < PLATFORMS_COUNT; i++) {
                    render_platform(&platforms[i], renderer);
                }

                render_player(&player, renderer);

                // Render projectiles
                if (player.can_shoot == false) {
                    render_bullet(&player.projectile, renderer);
                }

                player_display_hp(&player, renderer, WINDOW_WIDTH / 2 + 20, 10,
                                  WINDOW_WIDTH / 2 - 40);

                SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};
                char score_text[20];
                sprintf(score_text, "Score: %d", score);
                render_text(renderer, fonts.pixeled_small, 10, 0, c,
                            score_text);
                break;

            case GAMEOVER:
                render_gameover_screen(renderer, &screen_properties, &fonts,
                                       &cursors, score, WINDOW_WIDTH,
                                       WINDOW_HEIGHT);
                break;
            default:
                break;
        }

        SDL_SetCursor(screen_properties.cursor);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        // draw_circle(renderer, 300, 500, 10);

        // SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};
        // render_button(renderer, fonts.pixeled, 100, 100, c, "New game");

        SDL_RenderPresent(renderer);
    }

    // Uvolnění prostředků
    for (int i = 0; i < PLATFORMS_COUNT; i++) {
        free(platforms[i].parts);
    }
    free(enemy_bullets);
    for (int i = 0; i < spread_effects_length; i++) {
        free(spread_effects[i].particles);
    }
    free(spread_effects);
    free(screen_properties.buttons);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(fonts.pixeled);
    TTF_CloseFont(fonts.pixeled_small);
    TTF_Quit();
    SDL_Quit();

    return 0;
}