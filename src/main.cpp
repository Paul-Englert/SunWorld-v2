#include <iostream>

#include "../include/raylib.h"
#include "engine/assets.h"
#include "engine/timer.h"
#include "io/debug.h"
#include "gameplay/sunworld.h"

static constexpr int TICKS_PER_SECOND = 20;

int main() {

    InitWindow(1000, 800, "Sun World");

    {
        const int  display = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display));
        ToggleFullscreen();
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    Sunworld::Init();

    TickTimer timer(TICKS_PER_SECOND);

    while (!WindowShouldClose()) {

        if (timer.ShouldTick()) {

            Sunworld::Update();

        }

        BeginDrawing(); {

            Sunworld::Render(timer.GetPartialTick());
            
        } EndDrawing();

    }

    CloseWindow();

    return 0;
}