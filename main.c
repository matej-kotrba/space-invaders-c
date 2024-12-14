#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

#include "lib/game/effects/effects.h"
#include "lib/game/enemy/enemy.h"
#include "lib/game/platforms/platforms.h"
#include "lib/game/player/player.h"
#include "lib/game/projectile/projectile.h"
#include "lib/menu/button.h"
#include "lib/screens/screen_handler.h"
#include "lib/setup.h"
#include "lib/utils.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

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
    screen_properties.window = window;
    screen_properties.fonts = &fonts;
    screen_properties.cursors = &cursors;

    GameProperties gp;

    GameParams game_params = {.gp = &gp, .sp = &screen_properties};

    init_screen(get_active_screen(), &game_params);

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

        // SDL_SetCursor(cursors.def);
        // SDL_SetCursor(cursors.pointer);

        screen_properties.cursor = cursors.def;

        switch (get_active_screen()) {
            case GAME:
                // Game update cycle
                gp.seconds += delta_time;
                update_player(&gp.player, WINDOW_WIDTH, delta_time);
                try_shoot_player_projectile(&gp.player);

                if (gp.player.can_shoot == false) {
                    update_bullet(&gp.player.projectile, delta_time);
                }

                if (should_remove_bullet(&gp.player.projectile, WINDOW_WIDTH,
                                         WINDOW_HEIGHT)) {
                    reset_player_shot(&gp.player);
                }

                if (gp.player.can_shoot == false) {
                    for (int i = 0; i < PLATFORMS_COUNT; i++) {
                        int index = get_platform_bullet_hit_part_index(
                            &gp.platforms[i], &gp.player.projectile);
                        if (index == -1) continue;
                        platform_hit(&gp.platforms[i], &gp.player.projectile,
                                     index);
                        reset_player_shot(&gp.player);
                    }
                }

                for (int i = 0; i < gp.enemy_bullets_length; i++) {
                    update_bullet(&gp.enemy_bullets[i], delta_time);
                }

                for (int i = 0; i < gp.enemy_bullets_length; i++) {
                    if (is_bullet_on_player(&gp.enemy_bullets[i], &gp.player)) {
                        player_hit(&gp.player);
                        gp.enemy_bullets[i].should_delete = true;
                        if (gp.player.hp <= 0) {
                            set_active_screen(GAMEOVER, &game_params);
                            FILE* score_file = fopen("scoreboard.txt", "a");
                            if (score_file == NULL) {
                                printf("Error opening file!\n");
                            } else {
                                fprintf(score_file, "%d;%f\n", gp.score,
                                        gp.seconds);
                                fclose(score_file);
                            }
                        }
                        continue;
                    }

                    if (should_remove_bullet(&gp.enemy_bullets[i], WINDOW_WIDTH,
                                             WINDOW_HEIGHT)) {
                        gp.enemy_bullets[i].should_delete = true;
                        continue;
                    }

                    for (int j = 0; j < PLATFORMS_COUNT; j++) {
                        int index = get_platform_bullet_hit_part_index(
                            &gp.platforms[j], &gp.enemy_bullets[i]);
                        if (index == -1) continue;
                        platform_hit(&gp.platforms[j], &gp.enemy_bullets[i],
                                     index);
                        gp.enemy_bullets[i].should_delete = true;
                    }
                }

                for (int i = 0; i < gp.enemies_length; i++) {
                    update_enemy(&gp.enemies[i], WINDOW_WIDTH, delta_time);

                    if (should_spawn_bullet(&gp.enemies[i])) {
                        if (gp.enemy_bullets_length == gp.enemy_bullets_max) {
                            resize_array(gp.enemy_bullets, sizeof(Bullet),
                                         &gp.enemy_bullets_max,
                                         ENEMY_BULLET_ALLOC_COUNT);
                        }

                        Vector2 vec = get_default_bullet_size(ENEMY);
                        gp.enemy_bullets[gp.enemy_bullets_length] =
                            create_new_bullet(
                                gp.enemies[i].x + gp.enemies[i].w / 2,
                                gp.enemies[i].y + gp.enemies[i].h, vec.x, vec.y,
                                0, get_random_float(2, 3));

                        gp.enemies[i].shoot_delay = get_shoot_delay();
                        gp.enemy_bullets_length++;
                    }
                }

                if (gp.player.can_shoot == false) {
                    for (int i = 0; i < gp.enemies_length; i++) {
                        if (is_bullet_on_enemy(&gp.player.projectile,
                                               &gp.enemies[i])) {
                            if (gp.spread_effects_length ==
                                gp.spread_effects_max) {
                                resize_array(gp.spread_effects,
                                             sizeof(SpreadEffect),
                                             &gp.spread_effects_max,
                                             SPREAD_EFFECTS_ALLOC_COUNT);
                            }

                            gp.spread_effects[gp.spread_effects_length] =
                                create_new_spread_effect(
                                    gp.enemies[i].x + gp.enemies[i].w / 2,
                                    gp.enemies[i].y + gp.enemies[i].h / 2);

                            gp.spread_effects_length++;

                            enemy_hit(gp.enemies, &gp.enemies_length, i);
                            gp.player.can_shoot = true;
                            gp.score += ENEMY_SCORE;

                            i--;
                        }
                    }
                }

                for (int i = 0; i < gp.enemies_length; i++) {
                    float offset =
                        get_offset_over_border(&gp.enemies[i], WINDOW_WIDTH);
                    if (offset != 0) {
                        for (int j = 0; j < gp.enemies_length; j++) {
                            gp.enemies[j].xs *= -1;
                            gp.enemies[j].x += -offset;
                        }
                        break;
                    }
                }

                for (int i = 0; i < gp.spread_effects_length; i++) {
                    update_spread_effect(&gp.spread_effects[i], delta_time);
                    if (should_remove_spread_effect(&gp.spread_effects[i])) {
                        free(gp.spread_effects->particles);
                        remove_element(gp.spread_effects, sizeof(SpreadEffect),
                                       &gp.spread_effects_length, i);
                        i--;
                    }
                }

                for (int i = 0; i < gp.enemy_bullets_length; i++) {
                    if (gp.enemy_bullets[i].should_delete) {
                        remove_element(gp.enemy_bullets, sizeof(Bullet),
                                       &gp.enemy_bullets_length, i);
                    }
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                // Render effects
                for (int i = 0; i < gp.spread_effects_length; i++) {
                    render_spread_effect(&gp.spread_effects[i], renderer);
                }

                // Render Characters
                for (int i = 0; i < gp.enemies_length; i++) {
                    render_enemy(&(gp.enemies[i]), renderer);
                }

                for (int i = 0; i < gp.enemy_bullets_length; i++) {
                    render_bullet(&gp.enemy_bullets[i], renderer);
                }

                for (int i = 0; i < PLATFORMS_COUNT; i++) {
                    render_platform(&gp.platforms[i], renderer);
                }

                render_player(&gp.player, renderer);

                // Render projectiles
                if (gp.player.can_shoot == false) {
                    render_bullet(&gp.player.projectile, renderer);
                }

                player_display_hp(&gp.player, renderer, WINDOW_WIDTH / 2 + 20,
                                  10, WINDOW_WIDTH / 2 - 40);

                SDL_Color c = {.r = 255, .g = 255, .b = 255, .a = 255};
                char score_text[20];
                sprintf(score_text, "Score: %d", gp.score);
                render_text(renderer, fonts.pixeled_small, 10, 0, c,
                            score_text);
                break;

            case GAMEOVER:
                render_gameover_screen(renderer, &screen_properties, gp.score,
                                       gp.seconds);
                break;
            case MENU:
                render_menu_screen(renderer, &screen_properties);
                break;
            case SCOREBOARD:
                render_scoreboard_screen(renderer, &screen_properties);
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

        // Update input hold times etc.
        update_input_attrs(&inputs);
    }

    // Uvolnění prostředků
    for (int i = 0; i < PLATFORMS_COUNT; i++) {
        free(gp.platforms[i].parts);
    }
    free(gp.enemy_bullets);
    for (int i = 0; i < gp.spread_effects_length; i++) {
        free(gp.spread_effects[i].particles);
    }
    free(gp.spread_effects);
    free(screen_properties.buttons);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(fonts.pixeled);
    TTF_CloseFont(fonts.pixeled_small);
    TTF_Quit();
    SDL_Quit();

    return 0;
}