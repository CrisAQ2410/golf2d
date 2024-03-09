#ifndef OBSTACLES_RANDOM_H
#define OBSTACLES_RANDOM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "golf_game.h"

void generateRandomObstacles(Obstacle obstacles[], int numObstacles, SDL_Renderer* renderer);

#endif // OBSTACLES_RANDOM_H