#ifndef TIME_H
#define TIME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include "golf_game.h"
#include "mouse_handler.h"

using namespace std::chrono;
extern high_resolution_clock::time_point startTime;
const int GAME_DURATION = 30;

int calculateRemainingTime(high_resolution_clock::time_point startTime);

#endif // TIME_H