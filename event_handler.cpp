#include "event_handler.h"
#include "mouse_handler.h"
#include "restart.h"

void handleEvents(SDL_Event &e, bool &quit, bool &menuDisplayed, bool &isBallReleased, Ball &ball, Obstacle obstacles[], Hole &hole, int &strokes, int &score, bool &win, SDL_Renderer* renderer) {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        }
        if (menuDisplayed && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            menuDisplayed = false;
            isBallReleased = true;
            startTime = high_resolution_clock::now();
        }
        handleMouseEvents(e);
        if (!menuDisplayed && !win && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (mouseX >= WINDOW_WIDTH - 110 && mouseX <= WINDOW_WIDTH - 20 && mouseY >= 10 && mouseY <= 50) {
                resetGame(ball, obstacles, hole, score, strokes, win, renderer);
                strokes--;
            }
        }
        if (!menuDisplayed && e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            if (mouseX >= WINDOW_WIDTH - 50 && mouseX <= WINDOW_WIDTH - 10 && mouseY >= 10 && mouseY <= 50) {
                quit = true;
            }
        }
    }
}