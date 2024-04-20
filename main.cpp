#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
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
#include "render_game.h"
#include "render_menu.h"
#include "time.h"

using namespace std; 
using namespace std::chrono;

int remainingTime1 = calculateRemainingTime();

Ball ball;
Hole hole;
const int NUM_OBSTACLES = 24;
Obstacle obstacles[NUM_OBSTACLES];

TTF_Font *font = NULL;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Color textColor = {255, 255, 255};
bool isBallReleased = false;
bool isDragging = false;
int dragStartX, dragStartY;
int dragEndX, dragEndY;
float dragDistance = 0.0f;
float maxDragDistance = 200.0f;
int highestScore = 0;
int strokes = 0;
int score = 200;
bool win = false;
int logoWidth = 400;
int logoHeight = 350;
SDL_Event e;
bool quit = false;
bool menuDisplayed = true;

bool blink = false;
int blinkCounter = 0;
const int blinkThreshold = 30;

int main(int argc, char* args[]) {

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    window = SDL_CreateWindow("Golf 2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    font = TTF_OpenFont("font/font.ttf", 35);
    if (!font) {
        printf("Unable to load font: %s\n", TTF_GetError());
        return 1;
    }

    loadResources(renderer);

    srand(time(NULL));

    generateRandomObstacles(obstacles, NUM_OBSTACLES, renderer);

    generateRandomHole(hole, renderer);

    ball.x = rand() % WINDOW_WIDTH;
    ball.y = rand() % WINDOW_HEIGHT;
    ball.velX = 0;
    ball.velY = 0;
    SDL_QueryTexture(ballTexture, NULL, NULL, &ball.width, &ball.height);

    while (!quit) {

        handleEvents(e, quit, menuDisplayed, isBallReleased, ball, obstacles, hole, strokes, highestScore, score, win, renderer);
        
        blinkCounter++;

        if (blinkCounter >= blinkThreshold) {
            blink = !blink;
            blinkCounter = 0;
        }

        if (!menuDisplayed) {
            if (isBallReleased) {
                updateBallMovement(ball, obstacles, NUM_OBSTACLES, hole, collisionSound, holeSound, FRICTION, win);
            }
            renderGame(renderer, ball, hole, obstacles, NUM_OBSTACLES, isDragging, win, highestScore, score, menuDisplayed, blink, logoWidth, logoHeight, remainingTime1);
        }
        else {
            renderMenu(renderer, ball, hole, obstacles, NUM_OBSTACLES, isDragging, win, highestScore, score, menuDisplayed, blink, logoWidth, logoHeight);
        }

        if (!menuDisplayed && !win) {
            SDL_Rect restartRect = { WINDOW_WIDTH - 110, 10, 40, 40 };
            SDL_RenderCopy(renderer, restartTexture, NULL, &restartRect);
            SDL_Rect exitRect = { WINDOW_WIDTH - 50, 10, 40, 40 };
            SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
     
    SDL_DestroyTexture(logoTexture);
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(restartTexture);
    SDL_DestroyTexture(exitTexture); 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
