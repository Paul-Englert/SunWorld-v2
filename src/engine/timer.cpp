#include "timer.h"

#include "../io/debug.h"

#include <chrono>

long long TickTimer::Now() {

    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

}

TickTimer::TickTimer(int ticksPerSecond) {

    tickTime = 1000/ticksPerSecond;
    Reset();

}

void TickTimer::Reset() {

    lastTick = Now();

}

long long TickTimer::GetElapsedMillis() const {

    return Now() - lastTick;

}

bool TickTimer::ShouldTick() {

    const long long elapsed = GetElapsedMillis();

    if (elapsed >= tickTime) {

        lastTick = Now();
        return true;

    } else {

        return false;

    }

}

int TickTimer::GetElapsedTicks() {

    const int millisF = static_cast<int>(GetElapsedMillis());
    const int tickTime = static_cast<int>(this->tickTime);

    return millisF/tickTime;


}

float TickTimer::GetPartialTick() const {

    const float millisF = static_cast<float>(GetElapsedMillis());
    const float tickTimeF = static_cast<float>(tickTime);
    const float partial = millisF/tickTimeF;

    return partial;

}