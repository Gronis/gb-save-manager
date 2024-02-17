.include "hardware.inc"

.globl _main
.globl _STACK_PTR

.area _ENTRYPOINT
entrypoint:
    nop
    jp _main

.area _CODE

.globl _execute_vram_code
_execute_vram_code:
    jp _VRAM
