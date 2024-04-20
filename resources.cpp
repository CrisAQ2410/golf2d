#include "resources.h"

SDL_Texture* backgroundTexture = nullptr;
SDL_Texture* logoTexture = nullptr;
SDL_Texture* restartTexture = nullptr;
SDL_Texture* exitTexture = nullptr;
SDL_Texture* ballTexture = nullptr;
Mix_Chunk* holeSound = nullptr;
Mix_Chunk* collisionSound = nullptr;
Mix_Chunk* chargeSound = nullptr;


void loadResources(SDL_Renderer* renderer) {
    SDL_Surface* backgroundSurface = IMG_Load("img_src/bg.png");
    if (!backgroundSurface) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        return;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Surface* logoSurface = IMG_Load("img_src/logo.png");
    if (!logoSurface) {
        printf("Unable to load logo image: %s\n", IMG_GetError());
        return;
    }
    logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);
    SDL_FreeSurface(logoSurface);

    SDL_Surface* restartSurface = IMG_Load("img_src/restart.png");
    if (!restartSurface) {
        printf("Unable to load restart logo image: %s\n", IMG_GetError());
        return;
    }
    restartTexture = SDL_CreateTextureFromSurface(renderer, restartSurface);
    SDL_FreeSurface(restartSurface);

    SDL_Surface* exitSurface = IMG_Load("img_src/exit.png");
    if (!exitSurface) {
        printf("Unable to load exit icon image: %s\n", IMG_GetError());
        return;
    }
    exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    SDL_FreeSurface(exitSurface);

    SDL_Surface* ballSurface = IMG_Load("img_src/ball.png");
    if (!ballSurface) {
        printf("Unable to load ball image: %s\n", IMG_GetError());
        return;
    }
    ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface);

    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        printf("Mix_Init: %s\n", Mix_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
    }

    holeSound = Mix_LoadWAV("sound/hole.mp3");
    if (!holeSound) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }

    collisionSound = Mix_LoadWAV("sound/swing.mp3");
    if (!collisionSound) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }

    chargeSound = Mix_LoadWAV("sound/charge.mp3");
    if (!chargeSound) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }
}
