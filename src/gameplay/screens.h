#pragma once

#include "../engine/render.h"
#include "../engine/assets.h"

enum class FadeDirection {
    OUT, IN
};

/**
 * Ein Übergangsscreen zwischen einem neuen und einem alten Screen. Sollte nie direkt verwendet werden, stattdessen: Sunworld::SwitchScreen(new Screen(...), true).
 * 
 * Löscht den alten Screen, wenn er selbst gelöscht wird. Der neue bleibt bestehen.
 */
class ScreenTransition : public Screen {
    public:
        ScreenTransition(Screen *previous, Screen *next);
        virtual ~ScreenTransition() override;
        virtual void UpdateGameplay() override;
        virtual void RenderScreen(float partialTick) override;
    private:
        Screen *previous, *next;
        unsigned char alpha = 0;
        FadeDirection direction = FadeDirection::OUT;
};

class ScreenMainMenu : public Screen {
    public:
        ScreenMainMenu();
        virtual ~ScreenMainMenu() = default;
        virtual void UpdateGameplay() override;
        virtual void RenderScreen(float partialTick) override;
    private:
        AssetManager assetManager;
};