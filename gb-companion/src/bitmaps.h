#ifndef BITMAPS_H
#define BITMAPS_H

#include "graphics.h"

extern const uint8_t text_role[];
#define text_role_bitmap ((tile_bitmap_t*)&text_role)

extern const tile_bitmap_t smiley;
#define smiley_bitmap (&smiley)

extern const range_t tiles[];

#define empty_tile          (tiles[0])
#define smiley_tile         (tiles[1])
#define text_role_tile      (tiles[3])

#endif // BITMAPS_H