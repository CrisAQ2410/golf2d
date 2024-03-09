#ifndef PHYSICS_HANDLER_H
#define PHYSICS_HANDLER_H

#include <SDL2/SDL.h>
#include <cmath>
#include "golf_game.h"

extern bool isBallReleased;
extern Ball ball;

const float FRICTION = 0.98f;

void handlePhysics();

#endif // PHYSICS_HANDLER_H
