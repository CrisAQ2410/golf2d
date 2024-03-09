#ifndef MOUSE_HANDLE_H
#define MOUSE_HANDLE_H

#include <SDL2/SDL.h>
#include <cmath>
#include "golf_game.h"

extern bool isBallReleased;
extern bool isDragging;
extern int dragStartX, dragStartY;
extern int dragEndX, dragEndY;
extern float dragDistance;
extern float maxDragDistance;
extern Ball ball;

void handleMouseEvents(SDL_Event& e);

#endif // MOUSE_HANDLE_H
