#ifndef HOLE_RANDOM_H
#define HOLE_RANDOM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "golf_game.h"

void generateRandomHole(Hole& hole, SDL_Renderer* renderer);

#endif // HOLE_RANDOM_H