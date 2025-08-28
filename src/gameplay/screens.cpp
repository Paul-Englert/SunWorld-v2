#include "screens.h"

#include "sunworld.h"

#include "../io/debug.h"

#include "../../include/raylib.h"

/**
 * ScreenMainMenu
 */

ScreenMainMenu::ScreenMainMenu() : assetManager(Sunworld::GetCoreAssetManager()) {

    assetManager.AddSearchDir("assets/menu/");

}

void ScreenMainMenu::UpdateGameplay() {

    if (IsKeyDown(KEY_SPACE)) {

        Sunworld::SwitchScreen(new ScreenMainMenu());

    }

}

void ScreenMainMenu::RenderScreen(float partialTick) {

    ClearBackground(WHITE);

    Texture2D background = assetManager.GetTexture("background.png").value();
    FillScreenWithTexture(background);

    Texture2D logoTexture = assetManager.GetTexture("logo.png").value();
    const int x = GetRenderWidth()/2 - logoTexture.width/2;
    const int y = GetRenderHeight()/2 - logoTexture.height/2;
    DrawTexture(logoTexture, x, y, WHITE);

}