#include "physics_handler.h"

void handlePhysics(Ball& ball, bool& isBallReleased) {
    if (isBallReleased) {
        ball.x += ball.velX;
        ball.y += ball.velY;

        if (ball.x - ball.width / 2 < 0 || ball.x + ball.width / 2 > WINDOW_WIDTH) {
            ball.velX = -ball.velX;
        }
        if (ball.y - ball.height / 2 < 0 || ball.y + ball.height / 2 > WINDOW_HEIGHT) {
            ball.velY = -ball.velY;
        }
        if (fabs(ball.velX) < 0.1 && fabs(ball.velY) < 0.1) {
            isBallReleased = false;
        }

        ball.velX *= FRICTION;
        ball.velY *= FRICTION;
    }
}


void renderBall(SDL_Renderer* renderer, SDL_Texture* ballTexture, const Ball& ball) {
    SDL_Rect dstRect = {(int)(ball.x - ball.width / 2), (int)(ball.y - ball.height / 2), ball.width, ball.height};
    SDL_RenderCopy(renderer, ballTexture, NULL, &dstRect);
}
