#ifndef POWER_BAR_H
#define POWER_BAR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "golf_game.h"

extern int dragStartX, dragStartY;
extern int dragEndX, dragEndY;

void drawPowerBar(SDL_Renderer* renderer);

#endif // POWER_BAR_H