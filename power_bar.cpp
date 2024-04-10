#include "power_bar.h"

void drawPowerBar(SDL_Renderer* renderer, const Ball& ball) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderDrawLine(renderer, ball.x, ball.y, dragStartX - (dragEndX - dragStartX), dragStartY - (dragEndY - dragStartY));
}