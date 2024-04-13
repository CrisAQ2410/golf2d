#include "ball_movement.h"

void updateBallMovement(Ball &ball, Obstacle obstacles[], int NUM_OBSTACLES, Hole &hole, Mix_Chunk *collisionSound, Mix_Chunk *holeSound, const float FRICTION, bool &win) {
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