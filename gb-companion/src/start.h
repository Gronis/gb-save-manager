#ifndef START_H
#define START_H

#include "types.h"
#include "version.h"

// Use this function to call some other function while the screen
// is running in parallel. Typically use this to call code in RAM
//
// NOTE: Only call this function if RAM is available. This is not
// always the case!
//
// This is needed to fully escape from the default locked down
// environment where only video RAM (VRAM) and high RAM (HRAM)
// is accessible. When running code in RAM, we can show the
// screen while still executing code. This is not possible
// in the locked down environment where the code runs from VRAM
// (and a small amount from HRAM). VRAM is inaccessible while
// the hardware renders the screen, so in this mode, we have to
// toggle the screen on/off rapidly and do work between frames.
//
// The reason the locked down environment exists is that RAM
// is inacessible on GBA (in GBC mode) if the game cartridge
// is ejected. If we know we have access to RAM, we can escape
// and fully utilize the screen while doing work!
//
// This function does the following:
//
// Set the stack pointer to High RAM (0xFF80-0xFFFE)
// Enables the screen
// Calls the provided function pointer
// Disables the screen
// Reset the stack pointer to what it was before
volatile void run_in_parallel_to_screen(void (*function)(void));

// This does the same as `run_in_parallel_to_screen` except it
// is hard-coded to just wait roughly 2 frames before returning.
volatile void flush_screen(void);

// This function just loops cycles until its done.
// Function is located in HRAM so it can be used anytime (no
// timing requirements).
volatile void wait_n_cycles(uint16_t cycles);

// If we run inside VRAM, some functions will need to be copied to ram
// if not, then we are already running in vram and no copy is needed.
// #ifdef VRAM_VERSION
// #define copy_ram_functions_to_ram() copy_ram_functions_to_ram_impl()
// #endif
// #ifdef RAM_VERSION
// #define copy_ram_functions_to_ram()
// #endif

#define rDeviceModeBootup ((uint8_t*) 0xFFFC)

#endif // START_H
