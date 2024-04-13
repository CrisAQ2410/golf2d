#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <SDL2/SDL.h>
#include "golf_game.h"

void handleEvents(SDL_Event &e, bool &quit, bool &menuDisplayed, bool &isBallReleased, Ball &ball, Obstacle obstacles[], Hole &hole, int &strokes, int &score, bool &win, SDL_Renderer* renderer);

#endif //EVENT_HANDLER_H