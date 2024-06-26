#ifndef RESTART_H
#define RESTART_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "golf_game.h"
#include "hole_random.h"
#include "obstacles_random.h"
#include "time.h"

void resetGame(Ball& ball, Obstacle obstacles[], Hole& hole, int& score, int &strokes, int &highestScore, bool& win, SDL_Renderer* renderer);

#endif // RESTART_H