
// This makes sure we include the binary data from bitmaps here
#define INCLUDE_BIN_DATA
#include "bitmaps.h"
#include "graphics.h"

// Any code put in this file will be overwritten when rasterizing bitmap tiles,
// so it's a good idea to put initialization code here and call it asap when
// application has launched.
void render_header(void) {
    set_tiles_row(2, 0, *text_role_tile);
    set_tiles_row(2, 1, *text_cartridge_tile); set_tiles_row(8, 1, *text_state_tile);
    set_tiles_row(2, 2, *text_link_cable_tile); set_tiles_row(8, 2, *text_state_tile);
}