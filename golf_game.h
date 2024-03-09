#ifndef GOLF_GAME_H
#define GOLF_GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct Ball {
    float x, y;
    float velX, velY;
    int width, height;
    SDL_Texture* texture;
};

struct Obstacle {
    float x, y; 
    int width, height; 
    SDL_Texture* texture; 
};

struct Hole {
    float x, y; 
    int width, height; 
    SDL_Texture* texture; 
};

#endif // GOLF_GAME_H
