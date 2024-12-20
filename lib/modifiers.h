#pragma once

#define PLATFORMS_COUNT_MIN 0
#define PLATFORMS_COUNT_MAX 5
#define PLATFORMS_COUNT_DEFAULT 4

#define PLAYER_LIVES_MIN 1
#define PLAYER_LIVES_MAX 5
#define PLAYER_LIVES_DEFAULT 3

typedef struct {
    int max;
    int min;
    int current;
} ModifierInt;

typedef enum { PLATFORMS_COUNT, PLAYER_LIVES } ModifierIntType;

typedef struct {
    ModifierInt* modifiers_int;
    int modifiers_int_length;
} Modifiers;

Modifiers get_modifiers();
