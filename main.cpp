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

Ball ball;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
SDL_Color textColor = {255, 255, 255};
bool isBallReleased = false;
bool isDragging = false;
int dragStartX, dragStartY;
int dragEndX, dragEndY;
float dragDistance = 0.0f;
float maxDragDistance = 200.0f;
int strokes = 0;
int score = 200;

void renderText(SDL_Renderer* renderer, const char* text, SDL_Color color, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

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

    SDL_Surface* backgroundSurface = IMG_Load("img_src/bg.png");
    if (!backgroundSurface) {
        printf("Unable to load background image: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Surface* logoSurface = IMG_Load("img_src/logo.png");
    if (!logoSurface) {
        printf("Unable to load logo image: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* logoTexture = SDL_CreateTextureFromSurface(renderer, logoSurface);
    SDL_FreeSurface(logoSurface);

    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        printf("Mix_Init: %s\n", Mix_GetError());
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Mix_OpenAudio: %s\n", Mix_GetError());
    }

    Mix_Chunk* holeSound = Mix_LoadWAV("sound/hole.mp3");
    if (!holeSound) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }

    Mix_Chunk* collisionSound = Mix_LoadWAV("sound/swing.mp3");
    if (!collisionSound) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }

    Mix_Chunk* chargeSound = Mix_LoadWAV("sound/charge.mp3");
    if (!chargeSound) {
        printf("Mix_LoadWAV: %s\n", Mix_GetError());
    }

    srand(time(NULL));

    const int NUM_OBSTACLES = 24;
    Obstacle obstacles[NUM_OBSTACLES];

    generateRandomObstacles(obstacles, NUM_OBSTACLES, renderer);

    Hole hole;
    generateRandomHole(hole, renderer);

    SDL_Surface* ballSurface = IMG_Load("img_src/ball.png");
    if (!ballSurface) {
        printf("Unable to load ball image: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_FreeSurface(ballSurface);

    ball.x = rand() % WINDOW_WIDTH;
    ball.y = rand() % WINDOW_HEIGHT;
    ball.velX = 0;
    ball.velY = 0;
    SDL_QueryTexture(ballTexture, NULL, NULL, &ball.width, &ball.height);

    int logoWidth = 350;
    int logoHeight = 300;

    SDL_Event e;
    bool win = false;
    bool quit = false;
    bool menuDisplayed = true;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (menuDisplayed && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                menuDisplayed = false;
                isBallReleased = true;
            }
            handleMouseEvents(e);
        }
        
        if (!menuDisplayed) {
            if (isBallReleased) {
                ball.x += ball.velX;
                ball.y += ball.velY;
                if (ball.x - ball.width / 2 < 0 || ball.x + ball.width / 2 > WINDOW_WIDTH) {
                    ball.velX = -ball.velX;
                    Mix_PlayChannel(-1, collisionSound, 0);
                }
                if (ball.y - ball.height / 2 < 0 || ball.y + ball.height / 2 > WINDOW_HEIGHT) {
                    ball.velY = -ball.velY; 
                    Mix_PlayChannel(-1, collisionSound, 0);
                }
                if (fabs(ball.velX) < 0.1 && fabs(ball.velY) < 0.1) {
                    isBallReleased = false;
                }
                for (int i = 0; i < NUM_OBSTACLES; ++i) {
                    if (checkCollision(ball, obstacles[i])) {
                        Mix_PlayChannel(-1, collisionSound, 0);
                        handleCollision(ball, obstacles[i]);
                    }
                }
                if (checkHoleCollision(ball, hole)) {
                    Mix_PlayChannel(-1, holeSound, 0);
                    float distanceToHole = sqrt((ball.x - hole.x) * (ball.x - hole.x) + (ball.y - hole.y) * (ball.y - hole.y));
                    float angleToHole = atan2(hole.y - ball.y, hole.x - ball.x);

                    float speed = 5.0f;
                    ball.velX = speed * cos(angleToHole);
                    ball.velY = speed * sin(angleToHole);

                    ball.x += ball.velX;
                    ball.y += ball.velY;

                    if (distanceToHole < 10.0f) { 
                        ball.velX = 0;
                        ball.velY = 0;
                        win = true;
                    }
                }
                ball.velX *= FRICTION;
                ball.velY *= FRICTION;
            }
            
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
            for (int i = 0; i < NUM_OBSTACLES; ++i) {
                SDL_Rect obstacleRect = { (int)(obstacles[i].x - obstacles[i].width / 2), (int)(obstacles[i].y - obstacles[i].height / 2), obstacles[i].width, obstacles[i].height };
                SDL_RenderCopy(renderer, obstacles[i].texture, NULL, &obstacleRect);
            }
            SDL_Rect holeRect = { (int)(hole.x - hole.width / 2), (int)(hole.y - hole.height / 2), hole.width, hole.height };
            SDL_RenderCopy(renderer, hole.texture, NULL, &holeRect);
            SDL_Rect dstRect = { (int)(ball.x - ball.width / 2), (int)(ball.y - ball.height / 2), ball.width, ball.height };
            SDL_RenderCopy(renderer, ballTexture, NULL, &dstRect);
            std::string strokesText = "Strokes: " + std::to_string(strokes);
            std::string scoreText = "Score: " + std::to_string(score);
            renderText(renderer, strokesText.c_str(), textColor, 350, 1);
            renderText(renderer, scoreText.c_str(), textColor, 350, 565);
            if (win) {
                SDL_Rect frameRect = { WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
                SDL_SetRenderDrawColor(renderer, 0, 128, 0, 200);
                SDL_RenderFillRect(renderer, &frameRect);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &frameRect);
                std::string winText = "Congratulations! You Won!";
                std::string strokesText = "Your strokes: " + std::to_string(strokes);
                std::string scoreText = "Your score: " + std::to_string(score);
                renderText(renderer, winText.c_str(), textColor, WINDOW_WIDTH / 2 - 160, WINDOW_HEIGHT / 2 - 50);
                renderText(renderer, strokesText.c_str(), textColor, WINDOW_WIDTH / 2 - 90, WINDOW_HEIGHT / 2);
                renderText(renderer, scoreText.c_str(), textColor, WINDOW_WIDTH / 2 - 90, WINDOW_HEIGHT / 2 + 30);
            }
        }
        else {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
            SDL_Rect logoRect = {230, 50, logoWidth, logoHeight};
            SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
            renderText(renderer, "Left click anywhere to start", textColor, (WINDOW_WIDTH - 350) / 2, (WINDOW_HEIGHT + logoHeight) / 2 + 20);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
     
    SDL_DestroyTexture(logoTexture);
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}