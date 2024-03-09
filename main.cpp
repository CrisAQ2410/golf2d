#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "golf_game.h"
#include "mouse_handler.h"
#include "physics_handler.h"
#include "obstacles_random.h"

Ball ball;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool isBallReleased = false;
bool isDragging = false;
int dragStartX, dragStartY;
int dragEndX, dragEndY;
float dragDistance = 0.0f;
float maxDragDistance = 200.0f;

int main(int argc, char* args[]) {

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Golf 2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* backgroundSurface = IMG_Load("img_src/bg.png");
    if (!backgroundSurface) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    srand(time(NULL));

    const int NUM_OBSTACLES = 15;
    Obstacle obstacles[NUM_OBSTACLES];

    generateRandomObstacles(obstacles, NUM_OBSTACLES, renderer);

    //Hole hole;
    //generateRandomHole(hole, renderer);

    SDL_Surface* ballSurface = IMG_Load("img_src/ball.png");
    if (!ballSurface) {
        printf("Unable to load ball image: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface);

    ball.x = WINDOW_WIDTH / 2;
    ball.y = WINDOW_HEIGHT / 2;
    ball.velX = 0;
    ball.velY = 0;
    SDL_QueryTexture(ballTexture, NULL, NULL, &ball.width, &ball.height);

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            handleMouseEvents(e);
        }

        handlePhysics();

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        for (int i = 0; i < NUM_OBSTACLES; ++i) {
            SDL_Rect obstacleRect = { (int)(obstacles[i].x - obstacles[i].width / 2), (int)(obstacles[i].y - obstacles[i].height / 2), obstacles[i].width, obstacles[i].height };
            SDL_RenderCopy(renderer, obstacles[i].texture, NULL, &obstacleRect);
        }
        SDL_Rect dstRect = { (int)(ball.x - ball.width / 2), (int)(ball.y - ball.height / 2), ball.width, ball.height };
        SDL_RenderCopy(renderer, ballTexture, NULL, &dstRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
