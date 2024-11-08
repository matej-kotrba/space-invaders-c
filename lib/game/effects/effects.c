#include "effects.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../../utils.h"

#define PARTICLE_RADIUS 5

#define OPACITY_MAX 255

Particle create_new_particle(int x, int y, float sx, float sy) {
    Particle p = {.x = x, .y = y, .sx = sx, .sy = sy};
    return p;
}

SpreadEffect create_new_spread_effect(int x, int y) {
    float rx = get_random_float(-1, 1);
    float ry = get_random_float(-1, 1);

    SpreadEffect de = {.length = SPREAD_EFFECT_PARTICLE_COUNT,
                       .particles = (Particle*)malloc(
                           (sizeof(Particle) * SPREAD_EFFECT_PARTICLE_COUNT)),
                       .opacity = 255};

    for (int i = 0; i < de.length; i++) {
        rx = get_random_float(-1, 1);
        ry = get_random_float(-1, 1);

        de.particles[i] = create_new_particle(x, y, rx, ry);
    }

    return de;
}

void render_spread_effect(SpreadEffect* de, SDL_Renderer* renderer) {
    for (int i = 0; i < de->length; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, de->opacity);

        draw_circle(renderer, de->particles[i].x, de->particles[i].y,
                    PARTICLE_RADIUS);
    }
}

void update_spread_effect(SpreadEffect* de, float delta_time) {
    de->opacity -= 100 * delta_time;
    for (int i = 0; i < de->length; i++) {
        de->particles[i].x += de->particles[i].sx * 3;
        de->particles[i].y += de->particles[i].sy * 3;

        if (de->opacity < 0) de->opacity = 0;

        de->particles[i].sx *= 0.98;
        de->particles[i].sy *= 0.98;
    }
}

bool should_remove_spread_effect(SpreadEffect* de) { return de->opacity <= 0; }