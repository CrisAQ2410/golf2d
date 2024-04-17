#include "handle_collision.h"
#include <algorithm>

using namespace std;

#include "handle_collision.h"
#include <algorithm>

void handleCollision(Ball& ball, const Obstacle& obstacle) {
    // Tính toán vị trí mới của quả bóng sau mỗi bước di chuyển nhỏ
    while (true) {
        // Tính toán vị trí mới của quả bóng
        ball.x += ball.velX;
        ball.y += ball.velY;
        
        // Tính toán các biên của quả bóng sau mỗi bước di chuyển
        float ballLeft = ball.x - ball.width / 2;
        float ballRight = ball.x + ball.width / 2;
        float ballTop = ball.y - ball.height / 2;
        float ballBottom = ball.y + ball.height / 2;

        // Tính toán các biên của vật cản
        float obstacleLeft = obstacle.x - obstacle.width / 2;
        float obstacleRight = obstacle.x + obstacle.width / 2;
        float obstacleTop = obstacle.y - obstacle.height / 2;
        float obstacleBottom = obstacle.y + obstacle.height / 2;

        // Kiểm tra va chạm giữa quả bóng và vật cản
        if (ballRight >= obstacleLeft && ballLeft <= obstacleRight &&
            ballBottom >= obstacleTop && ballTop <= obstacleBottom) {
            // Xác định hướng va chạm
            float overlapX = std::min(ballRight, obstacleRight) - std::max(ballLeft, obstacleLeft);
            float overlapY = std::min(ballBottom, obstacleBottom) - std::max(ballTop, obstacleTop);

            float normalX = 0.0f, normalY = 0.0f;
            if (overlapX < overlapY) {
                normalX = (ball.x < obstacle.x) ? -1.0f : 1.0f;
            } else {
                normalY = (ball.y < obstacle.y) ? -1.0f : 1.0f;
            }

            // Tính toán vận tốc tương đối
            float relativeVelocity = ball.velX * normalX + ball.velY * normalY;
            
            // Điều chỉnh vận tốc của quả bóng sau va chạm (sử dụng hệ số đàn hồi)
            ball.velX -= 2.0f * relativeVelocity * normalX;
            ball.velY -= 2.0f * relativeVelocity * normalY;

            // Sau khi xử lý va chạm, thoát khỏi vòng lặp
            break;
        }
    }
}

