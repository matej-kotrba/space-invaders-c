#include "modifiers.h"

#include <stdio.h>

void save_modifiers(Modifiers* modifiers) {
    FILE* modificators_file = fopen("modificators.txt", "w");
    if (modificators_file == NULL) {
        printf("Error opening modifications file\n");
        return;
    }

    for (int i = 0; i < modifiers->modifiers_int_length; i++) {
        fprintf(modificators_file, "%d\n", modifiers->modifiers_int[i].current);
    }
}

Modifiers get_modifiers() {
    FILE* modificators_file = fopen("modificators.txt", "r");
    if (modificators_file == NULL) {
        printf("Error opening modifications file\n");
        return;
    }

    Modifiers modifiers;

    return modifiers;
}