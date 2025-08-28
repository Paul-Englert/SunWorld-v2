#pragma once

#include "../engine/render.h"
#include "../engine/assets.h"

class ScreenMainMenu : public Screen {
    public:
        ScreenMainMenu();
        virtual ~ScreenMainMenu() = default;
        virtual void UpdateGameplay() override;
        virtual void RenderScreen(float partialTick) override;
    private:
        AssetManager assetManager;
}; 