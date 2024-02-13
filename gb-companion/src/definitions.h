#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#if RAM_LOC == CODE_LOC
#define RAM_VERSION
#else
#define VRAM_VERSION
#endif

#endif // DEFINITIONS_H