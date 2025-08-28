#include "sunworld.h"

#include "../../include/raylib.h"

#include "../io/debug.h"

namespace Sunworld {

    struct {
        AssetManager coreAssetManager;
        FontRenderer fontRenderer{"assets/font/"};
        Screen *screen{nullptr};
    } State;

    void Init() {

        //alle Suchordner zu coreAssetManager hinzufügen
        {

        }

        State.screen = new ScreenMainMenu();

    }

    void Update() {

        State.screen->UpdateGameplay();

    }

    void Render(float partialTick) {

        State.screen->RenderScreen(partialTick);

    }

    void Exit(std::string message) {

        Debug::Log(Debug::LogLevel::FATAL, message.c_str());

        CloseWindow();
        Shutdown();

        std::exit(-1);

    }

    void Shutdown() {

    }

    void SwitchScreen(Screen *screen, bool transition) {

        if (transition) {

            Screen *current = State.screen;

            Screen *transitionScreen = new ScreenTransition(current, screen);

            State.screen = transitionScreen;

        } else {

            if (State.screen != nullptr) {

                delete State.screen;

            }

            State.screen = screen;

        }

    }

    FontRenderer *GetFontRenderer() {

        return &State.fontRenderer;

    }

    AssetManager *GetCoreAssetManager() {

        return &State.coreAssetManager;

    }

}