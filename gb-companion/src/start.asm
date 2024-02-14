.include "hardware.inc"

;##############################################################################
; Define constants and global variables
;##############################################################################
rAppSP              .equ 0xFFFD     ; This is where we save the main app stack pointer
                                    ; while executing HRAM code
_HRAM_STACK_PTR     .equ 0xFFFC     ; This is the start of HRAM stack pointer used
                                    ; each time the app moves control flow to HRAM

;##############################################################################
; Include functions
;##############################################################################
.globl _main

;##############################################################################
; CODE_LOC is address of program while executing on target (VRAM+HRAM)
;##############################################################################
.area _CODE_LOC                     ; Address: 0x0 while in ROM, or CODE_LOC after copy
CODE_LOC:
start_from_vram:                    ; Start of code execution when switching from GBA -> GB
    xor a
    ld  (rLCDC), a                  ; Turn off screen so that we can interact with VRAM properly
    jr  start

start_from_rom:                     ; Start of code execution when executing directly from ROM
    xor a
    ld  (rLCDC), a                  ; Initialize LCD control register

    ld  de, #0                      ; This is the ROM start location to copy from
copy_program_and_run:
    ld  hl, #CODE_LOC
copy_program_and_run_loop:
    ld  a, (de)
    inc de
    ld  (hl+), a
    ld  a, d
    cp  a, #0x20                    ; Code in RAM (max 8kB, the size of VRAM)
    jr  nz, copy_program_and_run_loop
    jp  CODE_LOC

start:
    ld  sp, #_STACK_PTR-1           ; Off by one, we don't want to overwrite first code instruction 
    xor a
    ld  (rSCX),a
    ld  (rSCY),a
    ld  a,#0x10                     ; read P15 - returns a, b, select, start
    ld  (rP1),a
    ld  a,#0x80
    ld  (#0xFF4C),a                 ; set as GBC+DMG

init_palette:                       ; load gbc palette colors (0: black, 1: white, 2: black, 3: white)
    ld a, #0xAA                     ; DMG palette (%11001100 => 3: opaque 2: transparent, 1: opaque, 0: transparent)
    ld (rBGP), a
    
    ld  a, #0x80                    ; enable auto increment when loading gbc palette
    ld  (rBCPS),a

    ld  a, #0xFF
    ld  (rBGPD),a                   ; color 0 p1: white 
    ld  a, #0x7F
    ld  (rBGPD),a                   ; color 0 p2: white 

    xor a
    ld  (rBGPD),a                   ; color 1 p1: black 
    ld  (rBGPD),a                   ; color 1 p2: black
   
    ld  a, #0xFF
    ld  (rBGPD),a                   ; color 2 p1: white 
    ld  a, #0x7F
    ld  (rBGPD),a                   ; color 2 p2: white 

    xor a
    ld  (rBGPD),a                   ; color 3 p1: black 
    ld  (rBGPD),a                   ; color 3 p2: black 

init_arrangements:
    ld  a, #0 -160 + 4              ; Move window x-axis to the far right     
    ld  (rSCX), a
    ld  a, #0 -144 + (160-144)*2 -4 ; Move window y-axis to bottom but compensate for affine reg zoom
    ld  (rSCY), a
    ; ld  de,#0x240
    ; ld  hl, #_SCRN1                 ; Use tiles at the start (for now)
    ld  b, #0
    ld  de, #32*11                  ; (32 tiles per line * 11 lines)
    ld  hl, #_SCRN1 +0x400 -#32*11  ; Use tiles at the end of VRAM (leave the rest for code)
init_arrangements_loop:
    ld  a,b
    ; inc b                           ; Set each tile to a new one
    ld (hl+),a
    dec de
    ld  a,d
    or  a,e
    jr  nz,init_arrangements_loop

; skip_clear_tiles_if_addr_inaccessible:
;     ld  hl, #_RAM
;     ld  (hl), #0x67
;     ld  a, (hl)
;     cp  a, #0x67
;     jr  z, copy_program_to_hram ; If we can read and write to addr, skip clear tile 0

clear_tile_zero:
    ld  hl, #_VRAM
    ld  a, #0x00
    ld  b, #16
clear_tile_zero_loop:
    ld  (hl+), a
    dec b
    jr  nz, clear_tile_zero_loop

copy_program_to_hram:
    ld  de, #hram_code              ; hram_code is already vram offset
    ld  hl, #_HRAM
copy_program_to_hram_loop:
    ld  a, (de)
    inc de
    ld  (hl+), a
    ld  a, l
    cp  a, #0xFE                    ; End of HRAM
    jr  nz, copy_program_to_hram_loop
    jp  _HRAM                       ; This moves execution from VRAM to HRAM

hram_code:
    jp _main

hram_wait_for_VBLANK:
    ld  hl, #rLY
    ld  a, (hl)
    cp  a, #144
    jr  nz, hram_wait_for_VBLANK
    ret

hram_wait_for_VRAM_accessible:
    ld  hl, #rSTAT
    bit 1,(hl)                                          ; Wait until Mode is 0
    jr  nz,hram_wait_for_VRAM_accessible
    ret

hram_wait:
    ld  de, #0x0A00                                     ; Timing is setup so LCD is on roughly 2 frames (15fps)
hram_wait_loop:
    dec de
    ld  a, d
    cp  a, #0
    jr  nz, hram_wait_loop
    ret

hram_flush_screen:
    ld hl, #hram_wait-#hram_code+#_HRAM
hram_run_in_parallel_to_screen:
    ld (rAppSP), sp                                     ; Save the stack pointer, since we have
    ld sp, #_HRAM_STACK_PTR                             ; to use HRAM for stack while LCD is on
hram_enable_screen:                                     ; VRAM becomes inaccessible after this point
    ld a, #LCDCF_ON | #LCDCF_BG8000 | #LCDCF_BG9C00 | #LCDCF_OBJ8 | #LCDCF_OBJOFF | #LCDCF_WINOFF | #LCDCF_BGON
    ld (rLCDC), a                                       ; setup screen to show img
hram_do_task:
    ld de, #hram_flush_screen_wait_for_screen_to_finish-hram_code+_HRAM
    push de                                             ; This is return addr since call r16 does not exist
    jp (hl)
hram_flush_screen_wait_for_screen_to_finish:            ; Could inline these functions, saving space and avoid chaning SP
    call hram_wait_for_VBLANK-hram_code+_HRAM           ; Wait for current frame to render (avoid tearing)
    call hram_wait_for_VRAM_accessible-hram_code+_HRAM
hram_disable_screen:
    xor a                                               ; Disable screen only if CODE_LOC is at VRAM
    ld (CODE_LOC-_VRAM+rLCDC), a                        ; else, if CODE_LOC is in RAM, will write to ROM addr (< 0x4000)
hram_reset_sp:
    ld sp, #rAppSP
    pop hl
    ld sp, hl
    ret

; Define math function just to make the code compile.
.globl __divuint
.globl __mulint
.globl __muluchar
.globl __mullong
.globl __modsint
.globl __moduint
.globl __divsint

__divuint:
__mulint:
__muluchar:
__mullong:
__modsint:
__moduint:
__divsint:
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; This is where we start execution when loading the ROM in a gb/gbc, has to be 4 bytes
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.area _ENTRYPOINT ; Address: 0x100 while in ROM, or CODE_LOC + 0x100 after move to RAM
entrypoint:
    nop
    jp start_from_rom-CODE_LOC  ; Must subtract CODE_LOC for correct address when code in ROM area

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; HRAM functions exposed to c main application
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.area _CODE

.globl _flush_screen
_flush_screen:
    jp hram_flush_screen-hram_code+_HRAM

.globl _run_in_parallel_to_screen
_run_in_parallel_to_screen:    ; function to call in reg de
	ld	l, e
    ld	h, d    ; move de to hl
    jp hram_run_in_parallel_to_screen-hram_code+_HRAM

end:; Important to have a non 0x00 or 0xFF in the end because bin2c strips away trailing data
    .db 0x99
