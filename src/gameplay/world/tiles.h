#pragma once

using TileID = char;
inline constexpr TileID INVALID_TILE_ID = -1;

class _Tile {

    private:
        TileID id = INVALID_TILE_ID;
};

using Tile = _Tile*;

Tile MakeTile();

namespace Tiles {



}