#include "physics_handler.h"
#include "check_collision.h"
#include "handle_collision.h"

void handlePhysics() {
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
    }


