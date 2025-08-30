#pragma once

#include "../../include/raylib.h"

class TickTimer final {
    public:
        TickTimer(int ticksPerSecond);
        ~TickTimer() = default;
        bool ShouldTick();
        int GetElapsedTicks();
        float GetPartialTick() const;
        void Reset();
        long long GetElapsedMillis() const;
        static long long Now();
    private:
        long long lastTick = -1;
        long long tickTime = -1;
};