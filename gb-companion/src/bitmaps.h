#ifndef BITMAPS_H
#define BITMAPS_H

#include "graphics.h"

extern const uint8_t text_role[];

extern const tile_bitmap_t smiley_bitmap;

extern const range_t tiles[];

#define empty (tiles[0])
#define smiley (tiles[1])
#define TEXT_ROLE (tiles[3])

#endif // BITMAPS_H