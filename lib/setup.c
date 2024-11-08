#include "setup.h"

#include <SDL2/SDL_ttf.h>

Fonts get_fonts() {
    Fonts fonts = {
        .pixeled = TTF_OpenFont(
            "../static/fonts/Pixelify_Sans/PixelifySans-Medium.ttf", 64)};

    return fonts;
}