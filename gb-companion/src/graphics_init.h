#ifndef GRAPHICS_INIT_H
#define GRAPHICS_INIT_H

#include "graphics.h"

// All code in this module is intended to be called once and then overwritten with
// graphics data.

// Render all messages in list. Note: messages should be placed consecutivly in memory
void render_message_no_screen_flush_call_only_before_rasterize(message_list_t* messages);

#endif // GRAPHICS_INIT_H