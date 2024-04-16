#include "power_bar.h"

void drawPowerBar(SDL_Renderer* renderer, const Ball& ball) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    int powerBarEndX = ball.x - (mouseX - dragStartX);
    int powerBarEndY = ball.y - (mouseY - dragStartY);

    SDL_RenderDrawLine(renderer, ball.x, ball.y, powerBarEndX, powerBarEndY);
}
