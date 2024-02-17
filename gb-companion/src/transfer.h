#ifndef TRANSFER_H
#define TRANSFER_H

#include "definitions.h"
#include "types.h"

// Store metadata at the end of RAM
#define rWorker     ((bool*)(_RAMBANK - 1))
#define rLeader     ((bool*)(_RAMBANK - 2))
#define rCGB_mode   ((bool*)(_RAMBANK - 3))
#define rAGB_mode   ((bool*)(_RAMBANK - 4))

#define LINK_CABLE_ENABLE_MASTER    0x81
#define LINK_CABLE_ENABLE           0x80
#define LINK_CABLE_NO_DATA          0xFF
#define LINK_CABLE_MAGIC_PACKET     0xAA

void ram_fn_transfer_header(void);

#endif // TRANSFER_H

