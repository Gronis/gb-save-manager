#include "transfer.h"
#include "hardware.h"
#include "graphics.h"

// This will ensure code is executable from RAM
#include "area_ram.h"

void wait_for_vblank_or_link_cable() {
    bool done;
    while (!(done = *rLY == 144));
}

void show_ram_is_working() {
    range_t* range = ((range_t*)(&tiles + pb_8_tile_index));
    wait_for_vblank_or_link_cable();
    set_tiles_row_repeat(14, 0, *range, 1);
}

void transfer_header(void) {

}