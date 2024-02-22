.include "hardware.inc"

;##############################################################################
; Define constants and global variables
;##############################################################################
rAppSP              .equ 0xFFFD     ; This is where we save the main app stack pointer
                                    ; while executing HRAM code

.globl              rDeviceModeBootup
rDeviceModeBootup   .equ 0xFFFC     ; This register holds if we are in AGB, CGB, GB, etc mode

_HRAM_STACK_PTR     .equ 0xFFFB     ; This is the start of HRAM stack pointer used
                                    ; each time the app moves control flow to HRAM

;##############################################################################
; Include functions
;##############################################################################
.globl _main

; These are Makefile definitions
; .globl VRAM1_LOC
.globl CODE

;##############################################################################
; CODE_LOC is address of program while executing on target (VRAM or RAM)
;##############################################################################
.area _CODE_LOC                     ; Address: 0x0 while in ROM, or CODE_LOC after copy
CODE_LOC:

start_from_vram:                    ; Start of code execution when code has been moved to VRAM
    xor a
    ld  (rLCDC), a                  ; Turn off screen so that we can interact with VRAM properly
    ld  sp, #_STACK_PTR-1           ; Off by one, we don't want to overwrite first code instruction
    
    ; We want to jump to start after mem_copy so push it to the stack.
    ld  hl, #start
    push hl

    ; Copy VRAM (0x8000) to CODE_LOC
    ld  de, #_VRAM                  ; src
    ld  hl, #CODE_LOC               ; dst
    ld  bc, #0x1000                 ; len

mem_copy:
    ld  a, (de)
    inc de
    ld  (hl+), a
    dec bc
    ld  a, b
    or  a, c
    jr  nz, mem_copy
    ret

start_from_rom:                     ; Start of code execution when executing directly from ROM
    xor a
    ld  (rLCDC), a                  ; Initialize LCD control register
    ld  sp, #_HRAM_STACK_PTR        ; Use HRAM stack pointer because we mess alot with VRAM here

    ; Copy ROM (0x0000) to VRAM (0x8000)
    ld  de, #0                      ; src
    ld  hl, #_VRAM                  ; dst
    ld  bc, #0x1000                 ; len
    push hl                         ; VRAM is where we want to start execution so push it to the stack.
    jr  mem_copy

start:
    xor a
    ld  (rSCX),a
    ld  (rSCY),a
    ld  a,#0x10                     ; read P15 - returns a, b, select, start
    ld  (rP1),a
    ld  a,#0x80
    ld  (#0xFF4C),a                 ; set as GBC+DMG

init_tile_palette:                  ; load gbc palette colors (0: black, 1: white, 2: black, 3: white)
    ld a, #0xCC                     ; DMG palette (%11001100 => 3: opaque 2: transparent, 1: opaque, 0: transparent)
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

init_screen_coordinates:
    ld  a, #0 -160 + 4              ; Move window x-axis to the far right     
    ld  (rSCX), a
    ld  a, #0 -144 - 4              ; Move window y-axis to bottom but compensate for affine reg zoom
    ld  (rSCY), a

rasterize_tile_zero:
    xor a
    ld  hl, #_VRAM
    ld  b, #16
rasterize_tile_zero_loop:
    ld  (hl+), a
    dec b
    jr  nz, rasterize_tile_zero_loop

copy_program_to_hram:
    ld  de, #hram_code              ; src
    ld  hl, #_HRAM                  ; dst
    ld  bc, #0xFFFB - #_HRAM        ; len, end of HRAM - some variables
    call mem_copy

start_program:
    jp  _main                       ; This moves execution from VRAM to HRAM


;##########################################################################
; wrap HRAM functions exposed to c main application
;##########################################################################

.globl _run_in_parallel_to_screen
_run_in_parallel_to_screen:    ; function to call in reg de
	ld	l, e
    ld	h, d    ; move de to hl
    jp hram_run_in_parallel_to_screen-hram_code+_HRAM

;##########################################################################
;  HRAM CODE
;##########################################################################

hram_code:

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
    push hl
    call hram_wait_for_VRAM_accessible-hram_code+_HRAM
    ld a, #LCDCF_ON | #LCDCF_BG8000 | #LCDCF_BG9C00 | #LCDCF_OBJ8 | #LCDCF_OBJOFF | #LCDCF_WINOFF | #LCDCF_BGON
    ld (rLCDC), a                                       ; setup screen to show img
    pop hl
hram_do_task:
    ld de, #hram_flush_screen_wait_for_screen_to_finish-hram_code+_HRAM
    push de                                             ; This is return addr since call r16 does not exist
    jp (hl)
hram_flush_screen_wait_for_screen_to_finish:            ; Could inline these functions, saving space and avoid chaning SP
    call hram_wait_for_VBLANK-hram_code+_HRAM           ; Wait for current frame to render (avoid tearing)
    call hram_wait_for_VRAM_accessible-hram_code+_HRAM
hram_disable_screen:
    xor a                                               ; Disable screen only if CODE_LOC is at VRAM
    ld (CODE_LOC-_VRAM9000+rLCDC), a                       ; else, if CODE_LOC is in RAM, will write to ROM addr (< 0x4000)
hram_reset_sp:
    ld sp, #rAppSP
    pop hl
    ld sp, hl
    ret

;##########################################################################
; This is where we start execution when loading the ROM in a gb/gbc, 
; has to be 4 bytes
;##########################################################################

; Address: 0x100 while in ROM, or CODE_LOC + 0x100 after move to RAM
.area _ENTRYPOINT
entrypoint:
    nop
    jp start_from_rom-CODE_LOC  ; Must offset CODE_LOC when code in ROM area

;##########################################################################
; define labels for HRAM functions exposed to c main application
;##########################################################################

.area _DABS (ABS)

.globl _flush_screen
.org hram_flush_screen-hram_code+_HRAM
_flush_screen:

.globl _wait_n_cycles
.org hram_wait_loop-hram_code+_HRAM
_wait_n_cycles:


;##########################################################################
; End of program
;##########################################################################
.area _CODE
.area _RAM_LOC
