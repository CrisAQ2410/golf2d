#ifndef BALL_MOVEMENT_H
#define BALL_MOVEMENT_H

#include "golf_game.h"
#include "obstacles_random.h"
#include "handle_collision.h"
#include "check_hole_collision.h"
#include "check_collision.h"
#include "resources.h"
#include <SDL2/SDL_mixer.h>

extern bool isBallReleased;

void updateBallMovement(Ball &ball, Obstacle obstacles[], int NUM_OBSTACLES, Hole &hole, Mix_Chunk *collisionSound, Mix_Chunk *holeSound, const float FRICTION, bool &win);

#endif // BALL_MOVEMENT_H