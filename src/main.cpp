#include <iostream>

#include "../include/raylib.h"
#include "engine/assets.h"
#include "io/debug.h"

int main() {

    Debug::Log(Debug::LogLevel::INFO, "Launching Sun World...");

    InitWindow(800, 400, "Sun World");

    ChangeDirectory("assets");
    Debug::Log(Debug::LogLevel::INFO, "Asset directory is: %s\n", GetWorkingDirectory());

    AssetManager assetManager;
    std::optional<Texture2D> texture = assetManager.GetTexture("logos.png");

    if (texture.has_value()) {
        Debug::Log(Debug::LogLevel::INFO, "Loaded logo texture");
    } else {
        Debug::Log(Debug::LogLevel::FATAL, "Could not find logo texture.");
        goto SHUTDOWN;
    }

    while (!WindowShouldClose()) {

        BeginDrawing();

        DrawTexture(texture.value(), 0, 0, WHITE);

        EndDrawing();

    }

    SHUTDOWN:
    Debug::Log("Exiting...");
    CloseWindow();

    return 0;
}