#ifndef VERSION_H
#define VERSION_H

#if RAM_LOC == CODE_LOC
#define RAM_VERSION
#else
#define VRAM_VERSION
#endif

#endif // VERSION_H