#include "check_collision.h"

bool checkCollision(const Ball& ball, const Obstacle& obstacle) {
    
    float ballLeft = ball.x - ball.width / 2;
    float ballRight = ball.x + ball.width / 2;
    float ballTop = ball.y - ball.height / 2;
    float ballBottom = ball.y + ball.height / 2;

    float obstacleLeft = obstacle.x - obstacle.width / 2;
    float obstacleRight = obstacle.x + obstacle.width / 2;
    float obstacleTop = obstacle.y - obstacle.height / 2;
    float obstacleBottom = obstacle.y + obstacle.height / 2;

    if (ballRight >= obstacleLeft && ballLeft <= obstacleRight &&
        ballBottom >= obstacleTop && ballTop <= obstacleBottom) {
        return true;
    }

    return false;
}