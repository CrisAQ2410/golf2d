#ifndef CHECK_COLLISION_H
#define CHECK_COLLISION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "golf_game.h"

bool checkCollision(const Ball& ball, const Obstacle& obstacle);

#endif // CHECK_COLLISION_H