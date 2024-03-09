#include "handle_collision.h"

void handleCollision(Ball& ball, const Obstacle& obstacle) {

    float dX = ball.x - obstacle.x;
    float dY = ball.y - obstacle.y;

    float distance = sqrt(dX * dX + dY * dY);

    float normalX = dX / distance;
    float normalY = dY / distance;

    float relativeVelocity = (ball.velX * normalX) + (ball.velY * normalY);

    ball.velX -= 2 * relativeVelocity * normalX;
    ball.velY -= 2 * relativeVelocity * normalY;
}