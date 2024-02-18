
#include "graphics_init.h"
#include "area_init.h"

#include "data.h"

void render_message_no_screen_flush_call_only_before_rasterize(message_list_t* messages) {
    uint8_t len = messages->len;
    messages++;
    for(uint8_t i = 0; i < len; ++i){
        message_t* m = (message_t*)(messages) + i;
        range_t* range = ((range_t*)(&tiles + m->message_tile_index));
        set_tiles_row(m->x, m->y, *range);
    }
}