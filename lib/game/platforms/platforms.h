typedef struct {
    int x;
    int y;
    int* parts;
} Platform;

void render_platform(Platform* platform, SDL_Renderer* renderer);