#ifndef TRANSFER_H
#define TRANSFER_H

#include "definitions.h"
#include "types.h"

// Store metadata at the end of RAM
#define rWorker     ((bool*)(_RAMBANK - 1))
#define rLeader     ((bool*)(_RAMBANK - 2))
#define rCGB_mode   ((bool*)(_RAMBANK - 3))
#define rAGB_mode   ((bool*)(_RAMBANK - 4))

void show_ram_is_working(void);

void transfer_header(void);

#endif // TRANSFER_H

