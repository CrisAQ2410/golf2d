#ifndef MOUSE_HANDLE_H
#define MOUSE_HANDLE_H

#include <SDL2/SDL.h>
#include <cmath>
#include "golf_game.h"
#include "restart.h"
#include "time.h"

extern bool isBallReleased;
extern bool isDragging;
extern int dragStartX, dragStartY;
extern int dragEndX, dragEndY;
extern float dragDistance;
extern float maxDragDistance;
extern Ball ball;
extern int highestScore;
extern int strokes;
extern int score;
extern SDL_Renderer* renderer;
extern Hole hole;
extern Obstacle obstacles[];
extern bool win;
extern int remainingTime1;

void handleMouseEvents(SDL_Event& e);

#endif // MOUSE_HANDLE_H
