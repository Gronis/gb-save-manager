#include "transfer.h"
#include "hardware.h"

// This will ensure code is executable from RAM
#include "area_ram.h"

void ram_function2() {
    return;
}

void ram_function() {
    ram_function2();
    return;
}
