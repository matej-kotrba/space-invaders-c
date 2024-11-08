#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define SPREAD_EFFECT_PARTICLE_COUNT 10

typedef struct {
    float x;
    float y;
    float sx;
    float sy;
} Particle;

typedef struct {
    int length;
    Particle* particles;
    int opacity;
} SpreadEffect;

Particle create_new_particle(int x, int y, float sx, float sy);
SpreadEffect create_new_spread_effect(int x, int y);

void render_spread_effect(SpreadEffect* de, SDL_Renderer* renderer);
void update_spread_effect(SpreadEffect* de, float delta_time);
bool should_remove_spread_effect(SpreadEffect* de);
