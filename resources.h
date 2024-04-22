#ifndef RESOURCES_H
#define RESOURCES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <cstdio>

extern SDL_Texture* backgroundTexture;
extern SDL_Texture* logoTexture;
extern SDL_Texture* restartTexture;
extern SDL_Texture* exitTexture;
extern SDL_Texture* ballTexture;
extern Mix_Chunk* holeSound;
extern Mix_Chunk* collisionSound;
extern Mix_Chunk* chargeSound;
extern Mix_Music* bgSound;

void loadResources(SDL_Renderer* renderer);

#endif // RESOURCES_H
