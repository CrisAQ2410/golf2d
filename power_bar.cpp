#include "power_bar.h"

void drawPowerBar(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderDrawLine(renderer, dragStartX, dragStartY, dragStartX - (dragEndX - dragStartX), dragStartY - (dragEndY - dragStartY));
}