#ifndef RENDER_TEXT_H
#define RENDER_TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "golf_game.h"

extern TTF_Font *font;

void renderText(SDL_Renderer* renderer, const char* text, SDL_Color color, int x, int y);

#endif // RENDER_TEXT_H