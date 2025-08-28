#include <iostream>

#include "../include/raylib.h"
#include "engine/assets.h"
#include "io/debug.h"

int main() {

    Debug::Log(Debug::LogLevel::INFO, "Launching Sun World...");

    InitWindow(800, 400, "Sun World");

    FontRenderer fontRenderer("assets/font/");

    while (!WindowShouldClose()) {

        BeginDrawing();
        {
            fontRenderer.DrawString("Sun World", {100, 100}, 5.3259f);
        }
        EndDrawing();

    }

    Debug::Log("Exiting...");
    CloseWindow();

    return 0;
}