#ifndef HANDLE_COLLISION_H
#define HANDLE_COLLISION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "golf_game.h"

void handleCollision(Ball& ball, const Obstacle& obstacle);

#endif // HANDLE_COLLISION_H