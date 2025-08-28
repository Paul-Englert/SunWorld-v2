#pragma once

#include "../../include/raylib.h"

class TickTimer final {
    public:
        TickTimer(int ticksPerSecond);
        ~TickTimer() = default;
        bool ShouldTick();
        float GetPartialTick() const;
        static long long Now();
    private:
        void Reset();
        long long GetElapsedMillis() const;
        long long lastTick;
        long long tickTime;
};