#include "restart.h"

void resetGame(Ball& ball, Obstacle obstacles[], Hole& hole, int& score, int& strokes, bool& win, SDL_Renderer* renderer) {
    ball.x = rand() % WINDOW_WIDTH;
    ball.y = rand() % WINDOW_HEIGHT; 
    ball.velX = 0; 
    ball.velY = 0;

    const int NUM_OBSTACLES = 24;
    const int GAME_DURATION = 30;

    generateRandomObstacles(obstacles, NUM_OBSTACLES, renderer);
    generateRandomHole(hole, renderer);

    score = 200;
    strokes = 0;
    win = false;

}