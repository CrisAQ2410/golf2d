#include "golf_game.h"
#include "mouse_handler.h"
#include "physics_handler.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Golf 2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    Ball ball;
    ball.x = WINDOW_WIDTH / 2;
    ball.y = WINDOW_HEIGHT / 2;
    ball.velX = 0;
    ball.velY = 0;


    SDL_QueryTexture(ball.texture, NULL, NULL, &ball.width, &ball.height);

    SDL_Event e;
    bool quit = false;
    bool isDragging = false;
    bool isBallReleased = false;
    int dragStartX, dragStartY;
    int dragEndX, dragEndY;
    float dragDistance = 0.0f;
    float maxDragDistance = 200.0f;

    while (!quit) {
        handleMouseEvents(e, ball, isDragging, isBallReleased, dragStartX, dragStartY, dragEndX, dragEndY, dragDistance, renderer);
        handlePhysics(ball, isBallReleased);
    }


    return 0;
}