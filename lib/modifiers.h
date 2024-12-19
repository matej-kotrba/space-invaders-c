#pragma once

typedef struct {
    int max;
    int min;
    int current;
} ModifierInt;

typedef enum {
  PLATFORMS_COUNT
} ModifierIntType;

typedef struct {
    ModifierInt* modifiers_int;
    int modifiers_int_length;
} Modifiers;

Modifiers* get_modifiers();
