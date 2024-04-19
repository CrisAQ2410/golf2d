#include "time.h"

duration<double> extraTimeToAdd(0);
bool extraTimeAdded = false;
high_resolution_clock::time_point startTime = high_resolution_clock::now();

int calculateRemainingTime() {
    high_resolution_clock::time_point currentTime = high_resolution_clock::now();
    duration<double> elapsedTime = duration_cast<seconds>(currentTime - startTime) - extraTimeToAdd;
    int remainingTime = GAME_DURATION - static_cast<int>(elapsedTime.count());
    if (remainingTime < 0) {
        remainingTime = 0;
    }
    if (win && !extraTimeAdded) {
        extraTimeToAdd += seconds(5);
        extraTimeAdded = true;
    }
    if (!win) {
        extraTimeAdded = false;
    }
    return remainingTime;
}
