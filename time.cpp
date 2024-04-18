#include "time.h"

int calculateRemainingTime() {
    high_resolution_clock::time_point currentTime = high_resolution_clock::now();
    duration<double> elapsedTime = duration_cast<seconds>(currentTime - startTime);
    int remainingTime = GAME_DURATION - static_cast<int>(elapsedTime.count());
    if (remainingTime < 0) {
        remainingTime = 0;
    }
    if (win) {
        remainingTime += 5;
    }
    return remainingTime;
}
