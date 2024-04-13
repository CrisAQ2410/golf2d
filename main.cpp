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
        handleEvents(e, quit, menuDisplayed, isBallReleased, ball, obstacles, hole, strokes, score, win, renderer);
        
        blinkCounter++;

        if (blinkCounter >= blinkThreshold) {
            blink = !blink;
            blinkCounter = 0;
        }

        if (!menuDisplayed) {
            if (isBallReleased) {
                updateBallMovement(ball, obstacles, NUM_OBSTACLES, hole, collisionSound, holeSound, FRICTION, win);
            }
            
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
            for (int i = 0; i < NUM_OBSTACLES; ++i) {
                SDL_Rect obstacleRect = { (int)(obstacles[i].x - obstacles[i].width / 2), (int)(obstacles[i].y - obstacles[i].height / 2), obstacles[i].width, obstacles[i].height };
                SDL_RenderCopy(renderer, obstacles[i].texture, NULL, &obstacleRect);
            }
            SDL_Rect holeRect = { (int)(hole.x - hole.width / 2), (int)(hole.y - hole.height / 2), hole.width, hole.height };
            SDL_RenderCopy(renderer, hole.texture, NULL, &holeRect);
            if (isDragging) {
                drawPowerBar(renderer, ball);
            }
            SDL_Rect dstRect = { (int)(ball.x - ball.width / 2), (int)(ball.y - ball.height / 2), ball.width, ball.height };
            SDL_RenderCopy(renderer, ballTexture, NULL, &dstRect);
            string strokesText = "Strokes: " + to_string(strokes);
            string scoreText = "Score: " + to_string(score);
            renderText(renderer, strokesText.c_str(), textColor, 350, 1);
            renderText(renderer, scoreText.c_str(), textColor, 350, 565);
            if (win) {
                SDL_Rect frameRect = { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
                SDL_SetRenderDrawColor(renderer, 0, 128, 0, 200);
                SDL_RenderFillRect(renderer, &frameRect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &frameRect);
                string restartText = "Press R to restart!";
                string winText = "Congratulations! You Won!";
                string strokesText = "Your strokes: " + to_string(strokes);
                string scoreText = "Your score: " + to_string(score);
                renderText(renderer, winText.c_str(), textColor, WINDOW_WIDTH / 2 - 155, WINDOW_HEIGHT / 2 - 80);
                renderText(renderer, strokesText.c_str(), textColor, WINDOW_WIDTH / 2 - 95, WINDOW_HEIGHT / 2 - 40);
                renderText(renderer, scoreText.c_str(), textColor, WINDOW_WIDTH / 2 - 95, WINDOW_HEIGHT / 2 );
                renderText(renderer, restartText.c_str(), textColor, WINDOW_WIDTH / 2 - 115, WINDOW_HEIGHT / 2 + 40);
            }
        }
        else {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
            SDL_Rect logoRect = {200, 50, logoWidth, logoHeight};
            SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
            SDL_Rect exitRect = { WINDOW_WIDTH - 50, 10, 40, 40 };
            SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
            if (blink) {
                renderText(renderer, "Left click anywhere to start", textColor, (WINDOW_WIDTH - 350) / 2, (WINDOW_HEIGHT + logoHeight) / 2 + 20);
            }
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
