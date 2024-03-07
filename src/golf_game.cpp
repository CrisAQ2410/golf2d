#include "golf_game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <cmath>

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    window = SDL_CreateWindow("Golf 2D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void loadTextures(SDL_Renderer* renderer, SDL_Texture*& backgroundTexture, SDL_Texture*& ballTexture) {
    SDL_Surface* backgroundSurface = IMG_Load("img_src/bg.png");
    if (!backgroundSurface) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        return;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Surface* ballSurface = IMG_Load("img_src/ball.png");
    if (!ballSurface) {
        printf("Unable to load ball image: %s\n", IMG_GetError());
        return;
    }
    ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface);
}

void closeSDL(SDL_Texture* backgroundTexture, SDL_Texture* ballTexture, SDL_Renderer* renderer, SDL_Window* window) {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void renderGame(SDL_Renderer* renderer, SDL_Texture* backgroundTexture, SDL_Texture* ballTexture, const Ball& ball) {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_Rect dstRect = { (int)(ball.x - ball.width / 2), (int)(ball.y - ball.height / 2), ball.width, ball.height };
    SDL_RenderCopy(renderer, ballTexture, NULL, &dstRect);

    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}
