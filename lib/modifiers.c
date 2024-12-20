#include "modifiers.h"

#include <stdio.h>
#include <stdlib.h>

void save_modifiers(Modifiers* modifiers) {
    FILE* modificators_file = fopen("modificators.txt", "w");
    if (modificators_file == NULL) {
        printf("Error opening modifications file\n");
        return;
    }

    for (int i = 0; i < modifiers->modifiers_int_length; i++) {
        fprintf(modificators_file, "%d\n", modifiers->modifiers_int[i].current);
    }

    fclose(modificators_file);
}

Modifiers get_modifiers() {
    FILE* modificators_file = fopen("modificators.txt", "r");

    Modifiers modifiers;
    modifiers.modifiers_int_length = 1;
    modifiers.modifiers_int = (ModifierInt*)malloc(
        modifiers.modifiers_int_length * sizeof(ModifierInt));

    modifiers.modifiers_int[PLATFORMS_COUNT].min = PLATFORMS_COUNT_MIN;
    modifiers.modifiers_int[PLATFORMS_COUNT].max = PLATFORMS_COUNT_MAX;

    if (modificators_file == NULL) {
        modifiers.modifiers_int[PLATFORMS_COUNT].current =
            PLATFORMS_COUNT_DEFAULT;

        save_modifiers(&modifiers);
    } else {
        for (int i = 0; i < modifiers.modifiers_int_length; i++) {
            fscanf(modificators_file, "%d",
                   &modifiers.modifiers_int[i].current);
        }
    }

    if (modificators_file != NULL) {
        fclose(modificators_file);
    }

    return modifiers;
}