## Supported or planned features:

### gb-save-manager.gb

Link Cable (2x GB/GBP/GBC/GBAs):
- (GB) SAVE Backup/Restore

~~Single GB/GBC/GBA:~~
- ~~(GB) QR SAVE Backup~~
- ~~(GB) Audio Cable SAVE Backup~~

### gb-save-manager.gba (Not started)

Link Cable (2x GBAs):
- (GB)  SAVE Backup/Restore
- (GBA) SAVE Backup/Restore

~~Single GBA:~~
- ~~(GBA) QR SAVE Backup~~
- ~~(GBA) Audio Cable SAVE Backup~~

## Developer NOTES

#### ROMs included in others ROMs:
- `gb-companion.gbc` Base ROM with all functionallity, runs in VRAM
- `gb-companion_ram.gbc` Same but runs in WRAM instead
- `gb-companion_mb.gba` Wraps `gb-companion.gbc` so it can be booted from gba mode
- `gba-companion_mb.gba` (Not started) Same as `gb-companion.gbc` but for gba cartridges

#### ROM inclusion structure:
- gb-save-manager.gb
  - gb-companion.gbc
  - gb-companion_ram.gbc
- gb-save-manager.gba
  - gb-companion_mb.gba
    - gb-companion.gbc
  - gba-companion_mb.gba

### Findings and solutions:

- GBA (while in gbc mode), cannot access data in WRAM if cartridge is disconnected (saftey feature?)
  - Solved by putting code in VRAM instead of WRAM
- VRAM can only be accessed at certain times in GBC mode
  - Solved by turing the screen off and on rapidly and put main logic in VRAM while HRAM and WRAM code can be used to render the screen.
- GB screen is too slow to toggle on and off fast
  - Solved by putting code in RAM instead of VRAM
- GBA data written to WRAM while cartridge is inserted is retained if a cartridge is ejected and then injected again.
  Seems to be controlled by the hardware switch used for detecting GBC games in cartridge slot. This means that we can
  execute code from RAM if cartridge is inserted and we know cartridge will not be ejected.

#### `gb-save-manager.gb` codeflow:
- Init steps:
  - Put gb-companion in VRAM and gb-companion_ram in WRAM
  - If started from GBA, switch execution to gb-companion (VRAM)
  - If started from GB/GBC switch execution to  gb-companion_ram (WRAM)
- Code runs as normal from RAM/VRAM until data transfer should start:
  - Switch execution to RAM which handles data transfer for both leader and worker
