#pragma once

#define PLATFORMS_COUNT_MIN 0
#define PLATFORMS_COUNT_MAX 5
#define PLATFORMS_COUNT_DEFAULT 4

typedef struct {
    int max;
    int min;
    int current;
} ModifierInt;

typedef enum { PLATFORMS_COUNT } ModifierIntType;

typedef struct {
    ModifierInt* modifiers_int;
    int modifiers_int_length;
} Modifiers;

Modifiers get_modifiers();
