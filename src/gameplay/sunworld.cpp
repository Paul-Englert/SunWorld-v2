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

            enum class FadeDirection {
                OUT, IN
            };

            class ScreenTransition : public Screen {
                public:
                    ScreenTransition(Screen *previous, Screen *next) {

                        this->previous = previous;
                        this->next = next;

                    }
                    virtual ~ScreenTransition() override {

                        delete previous;

                    }
                    virtual void UpdateGameplay() override {

                        #define ALPHA_STEP 10

                        if (direction == FadeDirection::OUT) {

                            int newAlpha = alpha + ALPHA_STEP;
                            if (newAlpha >= 255) {
                                newAlpha = 255;
                            }

                            if (newAlpha == 255) {
                                direction = FadeDirection::IN;
                            }

                            alpha = newAlpha;

                        } else if (direction == FadeDirection::IN) {

                            int newAlpha = alpha - ALPHA_STEP;
                            if (newAlpha <= 0) {
                                newAlpha = 0;
                            }

                            if (newAlpha == 0) {
                                Sunworld::SwitchScreen(next, false);
                            }

                            alpha = newAlpha;

                        }

                        #undef ALPHA_STEP
                        
                    }
                    virtual void RenderScreen(float partialTick) override {
                        if (direction == FadeDirection::OUT) {

                            previous->RenderScreen(partialTick);

                        } else if (direction == FadeDirection::IN) {

                            next->RenderScreen(partialTick);

                        }

                        const Color c{0, 0, 0, alpha};
                        DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), c);

                    }
                private:
                    Screen *previous, *next;
                    unsigned char alpha = 0;
                    FadeDirection direction = FadeDirection::OUT;
            };

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