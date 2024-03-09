#include "check_hole_collision.h"

bool checkHoleCollision(const Ball& ball, const Hole& hole) {
    int ballLeft = ball.x - ball.width / 2;
    int ballRight = ball.x + ball.width / 2;
    int ballTop = ball.y - ball.height / 2;
    int ballBottom = ball.y + ball.height / 2;

    int holeLeft = hole.x - hole.width / 2;
    int holeRight = hole.x + hole.width / 2;
    int holeTop = hole.y - hole.height / 2;
    int holeBottom = hole.y + hole.height / 2;

    if (ballRight >= holeLeft && ballLeft <= holeRight &&
        ballBottom >= holeTop && ballTop <= holeBottom) {
        return true;
    }

    return false;
}