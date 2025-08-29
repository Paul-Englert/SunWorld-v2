#include "tiles.h"

#include "../../engine/allocator.h"

#include <unordered_map>

void MakeTile() {

    static ArenaAllocator<sizeof(_Tile)*255> tileAllocator;

    Tile t = tileAllocator.Alloc<_Tile>();

    return t;
}