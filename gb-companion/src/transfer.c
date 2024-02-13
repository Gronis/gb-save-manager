#include "transfer.h"
#include "hardware.h"

// This will ensure code is executable from RAM
#include "area_ram.h"

// Most of this code is dummy code just to test if RAM <-> VRAM switching works
typedef struct {
    uint8_t line[8];
} tile_t;

void ram_rasterize_tile(uint8_t id, tile_t* tile) {
    uint16_t _id = ((uint16_t)id) * 16;
    for (uint8_t i = 0; i < 8; i++){
        uint8_t line = tile->line[i];
        _VRAM[_id] = line;
        // Skip second byte (colors 0/2). Can be any value because of palette
        _id+= 2; 
    }
}


const tile_t checker = {
    {
        0x55,
        0xAA,
        0x55,
        0xAA,
        0x55,
        0xAA,
        0x55,
        0xAA,
    }
};
void ram_tile_to_checker() {
    ram_rasterize_tile(0, &checker);
}

const tile_t empty = {
    {
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
        0xFF,
    }
};
void ram_tile_to_empty() {
    ram_rasterize_tile(0, &empty);
}
