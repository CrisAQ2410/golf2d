#include "handle_collision.h"
#include <algorithm>

using namespace std;

void handleCollision(Ball& ball, const Obstacle& obstacle) {
    ball.x += ball.velX;
    ball.y += ball.velY;
    
    int ballLeft = ball.x - ball.width / 2;
    int ballRight = ball.x + ball.width / 2;
    int ballTop = ball.y - ball.height / 2;
    int ballBottom = ball.y + ball.height / 2;

    int obstacleLeft = obstacle.x - obstacle.width / 2;
    int obstacleRight = obstacle.x + obstacle.width / 2;
    int obstacleTop = obstacle.y - obstacle.height / 2;
    int obstacleBottom = obstacle.y + obstacle.height / 2;

    if (ballRight >= obstacleLeft && ballLeft <= obstacleRight &&
        ballBottom >= obstacleTop && ballTop <= obstacleBottom) {

        float overlapX = min(ballRight, obstacleRight) - max(ballLeft, obstacleLeft);
        float overlapY = min(ballBottom, obstacleBottom) - max(ballTop, obstacleTop);

        float normalX = 0.0f, normalY = 0.0f;
        if (overlapX < overlapY) {
            normalX = (ball.x < obstacle.x) ? -1.0f : 1.0f;
        } else {
            normalY = (ball.y < obstacle.y) ? -1.0f : 1.0f;
        }

        float relativeVelocity = ball.velX * normalX + ball.velY * normalY;

        ball.velX -= 2.0f * relativeVelocity * normalX;
        ball.velY -= 2.0f * relativeVelocity * normalY;
    }
}

