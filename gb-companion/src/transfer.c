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

#define CARTRIDGE_TITLE ((char*)(0x0134))
const char LEADER_CARTRIDGE_TITLE[] = "GBSAVEMANAGER";
#define CARTRIDGE_LOGO ((char*)(0x0104))
const uint8_t CORPORATE_LOGO[] = {
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C
};

bool is_leader_cartridge_inserted() {
    bool val = true;
    for (uint8_t i = 0; i < sizeof(LEADER_CARTRIDGE_TITLE) - 1; ++i){
        if (CARTRIDGE_TITLE[i] != LEADER_CARTRIDGE_TITLE[i]
          || CARTRIDGE_LOGO[i] != CORPORATE_LOGO[i]){
            val = false;
            break;
        }
    }
    return val;
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
    if(is_leader_cartridge_inserted()){
        ram_rasterize_tile(0, &checker);
    }
}
