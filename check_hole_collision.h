#ifndef CHECK_HOLE_COLLISION_H
#define CHECK_HOLE_COLLISION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "golf_game.h"

bool checkHoleCollision(const Ball& ball, const Hole& hole);

#endif // CHECK_HOEL_COLLISION_H