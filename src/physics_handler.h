#ifndef PHYSICS_HANDLER_H
#define PHYSICS_HANDLER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include "golf_game.h"

void handlePhysics(Ball& ball, bool& isBallReleased);

#endif /*PHYSICS_HANDLER_H*/
