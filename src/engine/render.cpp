#include "render.h"

void DrawTexturedRect(Texture2D texture, Rectangle rect) {

    const Rectangle sourceRec{0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
    constexpr Vector2 origin{0, 0};
    constexpr float rotation{0};
    constexpr Color NO_TINT = WHITE;

    DrawTexturePro(texture, sourceRec, rect, origin, rotation, NO_TINT);

}

void FillScreenWithTexture(Texture2D texture) {

    DrawTexturedRect(texture, {0, 0, static_cast<float>(GetRenderWidth()), static_cast<float>(GetRenderHeight())});

}