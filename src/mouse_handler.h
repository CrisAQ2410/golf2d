#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include "golf_game.h"

void handleMouseEvents(SDL_Event& e, Ball& ball, bool& isDragging, bool& isBallReleased, int& dragStartX, int& dragStartY, int& dragEndX, int& dragEndY, float& dragDistance, SDL_Renderer* renderer);

#endif  /* MOUSE_HANDLER_H */
