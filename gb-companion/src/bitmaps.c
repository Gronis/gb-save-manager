
#include "bitmaps.h"

#include "text_role_bin.h"

#define SINGLE_TILE_LEN             (1)

#define EMPTY_TILE_B                (0)
#define EMPTY_TILE_E                ((EMPTY_TILE_B) + (SINGLE_TILE_LEN))

#define SMILEY_TILE_B               (EMPTY_TILE_E)
#define SMILEY_TILE_E               ((SMILEY_TILE_B) + (SINGLE_TILE_LEN))

#define TEXT_ROLE_B                 (SMILEY_TILE_E)
#define TEXT_ROLE_E                 ((TEXT_ROLE_B) + (text_role_length / 8))


const tile_bitmap_t smiley = {
    {// line:
        0x3C,  //   ####
        0x42,  //  #    #
        0xA5,  // # #  # #
        0x81,  // #      #
        0xA5,  // # #  # #
        0x99,  // #  ##  #
        0x42,  //  #    #
        0x3C,  //   ####
    }
};

const range_t tiles[] = {
    { EMPTY_TILE_B,         EMPTY_TILE_E},
    { SMILEY_TILE_B,        SMILEY_TILE_E},
    { EMPTY_TILE_B,         EMPTY_TILE_E},
    { TEXT_ROLE_B,          TEXT_ROLE_E},
};