#pragma once

#include "../engine/assets.h"

#include "screens.h"

namespace Sunworld {

    void Init();

    void Update();

    void Render(float partialTick);

    void Exit(std::string message = "Exited abnormally, did something go wrong?");

    /**
     * Wird in main() als Teil des normalen Shutdown-Prozederes aufgerufen.
     */
    void Shutdown();

    /**
     * Setzt einen neuen screen.
     * Ownership des Screen-Zeigers wird an diese Funktion übergeben.
     */
    void SwitchScreen(Screen *screen, bool transition = true);

    FontRenderer *GetFontRenderer();

    AssetManager *GetCoreAssetManager();

}