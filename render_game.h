#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "golf_game.h"
#include "mouse_handler.h"
#include "obstacles_random.h"
#include "check_collision.h"
#include "handle_collision.h"
#include "hole_random.h"
#include "check_hole_collision.h"
#include "render_text.h"
#include "power_bar.h"
#include "resources.h"
#include "ball_movement.h"
#include "event_handler.h"

using namespace std;

extern SDL_Color textColor;

void renderGame(SDL_Renderer* renderer, Ball ball, Hole hole, Obstacle obstacles[], int NUM_OBSTACLES, bool isDragging, bool win, int strokes, int score, bool menuDisplayed, bool blink, int logoWidth, int logoHeight);

#endif // RENDER_H