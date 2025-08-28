#pragma once

#include "../../include/raylib.h"

class Screen {
    public:
        virtual ~Screen() = default;
        virtual void RenderScreen(float partialTick) = 0;
        virtual void UpdateGameplay() = 0;
};

void DrawTexturedRect(Texture2D texture, Rectangle rect);

void FillScreenWithTexture(Texture2D texture);