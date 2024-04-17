#include "mouse_handler.h"

int previousHighestScore = 0;

void handleMouseEvents(SDL_Event& e) {
    if (e.type == SDL_MOUSEBUTTONDOWN && !isDragging && !isBallReleased) {
        isDragging = true;
        SDL_GetMouseState(&dragStartX, &dragStartY);
    } else if (e.type == SDL_MOUSEMOTION && isDragging) {
        SDL_GetMouseState(&dragEndX, &dragEndY);
        dragDistance = std::hypot(dragEndX - dragStartX, dragEndY - dragStartY);

        float angle = atan2(dragEndY - dragStartY, dragEndX - dragStartX);
        ball.velX = -dragDistance * cos(angle) / 10.0f;
        ball.velY = -dragDistance * sin(angle) / 10.0f;
    } else if (e.type == SDL_MOUSEBUTTONUP && isDragging) {
        isDragging = false;
        isBallReleased = true;
        dragDistance = 0.0f;
        strokes ++;
        if (strokes > 1) score -=5;
    } else if (e.type == SDL_KEYDOWN) {
        int remainingTime = calculateRemainingTime(startTime);
        if (e.key.keysym.sym == SDLK_r && remainingTime == 0) {
            resetGame(ball, obstacles, hole, score, strokes, highestScore, win, renderer);
            startTime = high_resolution_clock::now();
            highestScore = 0;
        }
    } else if (win) {
        int remainingTime = calculateRemainingTime(startTime);
        previousHighestScore = score;
        resetGame(ball, obstacles, hole, score, strokes, highestScore, win, renderer);
        highestScore += previousHighestScore;
        remainingTime += 5;
    }
}
