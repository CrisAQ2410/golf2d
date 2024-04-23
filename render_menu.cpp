#include "render_menu.h"

void renderMenu(SDL_Renderer* renderer, Ball ball, Hole hole, Obstacle obstacles[], int NUM_OBSTACLES, bool isDragging, bool win, int strokes, int score, bool menuDisplayed, bool blink, int logoWidth, int logoHeight) {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_Rect logoRect = {200, 50, logoWidth, logoHeight};
    SDL_RenderCopy(renderer, logoTexture, NULL, &logoRect);
    SDL_Rect exitRect = { WINDOW_WIDTH - 50, 10, 40, 40 };
    SDL_RenderCopy(renderer, exitTexture, NULL, &exitRect);
    if (blink) {
        renderText(renderer, "Left click anywhere to start", textColor, (WINDOW_WIDTH - 350) / 2, (WINDOW_HEIGHT + logoHeight) / 2 + 20);
    } 
}