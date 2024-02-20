#ifndef ARRAY_H
#define ARRAY_H

#include "types.h"

extern const uint8_t vram1_code[];
extern const uint8_t ram_code[];

#include "vram1_code_gbc.h"

#ifdef VRAM_VERSION
#include "ram_code_gbc.h"
#endif 

#endif // ARRAY_H