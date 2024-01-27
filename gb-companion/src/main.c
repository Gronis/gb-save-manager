#include "types.h"
#include "hardware.h"

#include "qr.h"

typedef struct {
    uint8_t line[8];
} tile_t;

void rasterize_tile(uint8_t id, tile_t* tile) {
    uint16_t _id = ((uint16_t)id) * 16;
    for (uint8_t i = 0; i < 8; i++){
        uint8_t line = tile->line[i];
        _VRAM[_id] = line;
        _id++;
        _VRAM[_id] = line;
        _id++;
    }
}

void set_bkg_tiles(uint8_t x, uint8_t y, uint8_t width, uint8_t height, const uint8_t* tiles) {
    uint8_t row, col;
    uint16_t index;
    uint8_t row_times_width = 0;
    for (row = 0; row < height; row++) {
        for (col = 0; col < width; col++) {
            // Calculate the index in the BG Map based on (x, y) coordinates
            index = (y + row) * 32 + (x + col);
            _SCRN1[index] = tiles[row_times_width + col];
        }
        row_times_width += width;
    }
}

void main() {
    QRCode qrcode;
    uint8_t qrcodeBytes[QRCODE_BUFFER_SIZE(13)];
    tile_t smiley = {
        {// line:
            0x3C,
            0x42,
            0xA5,
            0x81,
            0xA5,
            0x99,
            0x42,
            0x3C,
        }
    };
    // qrcode_initText(&qrcode, qrcodeBytes, 3, ECC_LOW, "HELLO WORLD");
    rasterize_tile(1, &smiley);
    {
        uint8_t corners_x[4] = {
            32-14, 32-6, 32-6, 32-14,
        };
        uint8_t corners_y[4] = {
            32-10, 32-10, 32-2, 32-2,
        };
        uint8_t tile_id = 0;
        // Infinite loop to keep the program running
        while (1) {
            tile_id = (tile_id + 1) & 1;
            for (uint8_t i = 0; i <= 4; ++i){
                set_bkg_tiles(corners_x[i], corners_y[i], 1, 1, &tile_id);
            }
            // blink every other second (at 30 fps)
            for (uint8_t i = 0; i <= 15; ++i){
                flush_screen();
            }
        }
    }
}
