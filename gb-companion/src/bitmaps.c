
#include "bitmaps.h"

#include "text_role_bin.h"

const tile_bitmap_t smiley_bitmap = {
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
    { 0, 0 + 1 },
    { 1, 1 + 1 },
    { 0, 0 + 1 },
    { 3, 3 + text_role_length / 8 },
};