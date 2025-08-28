#include "screens.h"

#include "sunworld.h"

#include "../io/debug.h"

#include "../../include/raylib.h"

/**
 * ScreenTransition
 */

ScreenTransition::ScreenTransition(Screen *previous, Screen *next) {
    this->previous = previous;
    this->next = next;
}

ScreenTransition::~ScreenTransition() {

    delete previous;

}

void ScreenTransition::RenderScreen(float partialTick) {

    if (direction == FadeDirection::OUT) {

        previous->RenderScreen(partialTick);

    } else if (direction == FadeDirection::IN) {

        next->RenderScreen(partialTick);

    }

    const Color c{0, 0, 0, alpha};
    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), c);

}

void ScreenTransition::UpdateGameplay() {

    #define ALPHA_STEP 5

    if (direction == FadeDirection::OUT) {

        alpha += ALPHA_STEP;

        if (alpha >= 255) {

            alpha = 255;
            direction = FadeDirection::IN;

        }

    } else if (direction == FadeDirection::IN) {

        alpha -= ALPHA_STEP;

        if (alpha <= 0) {

            alpha = 0;
            Sunworld::SwitchScreen(next, false);

        }

    }

    #undef ALPHA_STEP

}

/**
 * ScreenMainMenu
 */

ScreenMainMenu::ScreenMainMenu() : assetManager(Sunworld::GetCoreAssetManager()) {

    assetManager.AddSearchDir("assets/menu/");

}

void ScreenMainMenu::UpdateGameplay() {

    

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