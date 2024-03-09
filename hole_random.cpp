#include "hole_random.h"

void generateRandomHole(Hole& hole, SDL_Renderer* renderer) {

    SDL_Surface* holeSurface = IMG_Load("img_src/hole.png");
    if (!holeSurface) {
        printf("Unable to load hole image: %s\n", IMG_GetError());
        exit(1);
    }
    hole.texture = SDL_CreateTextureFromSurface(renderer, holeSurface);
    SDL_FreeSurface(holeSurface);

    hole.x = rand() % WINDOW_WIDTH;
    hole.y = rand() % WINDOW_HEIGHT;
    hole.width = 20; 
    hole.height = 20;
}