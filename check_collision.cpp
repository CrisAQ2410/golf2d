#include "check_collision.h"

bool checkCollision(const Ball& ball, const Obstacle& obstacle) {
    
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
        return true;
    }

    return false;
}