## 2-system Menu (GB and GBA works the same or very similar)

### Screens:

#### Boot Screen: Choose Cartridge Mode (GBA only)
The user have to choose between GB and GBA mode:
```
┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃   Role: Leader                 ┃
┃   Link Cable Status:     ✖️     ┃
┃   Cartridge  Status:     ✔     ┃
┃                                ┃
┃   Press Button to use:         ┃
┃                                ┃
┃   A: Enter GBA Mode            ┃
┃   B: Enter GB/GBC Mode         ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
```
#### Waiting for Link or role change (GBA only)
Help text will be specific on GBA since GBA will send Worker app with Multiboot:
```
┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃   Role: Leader                 ┃
┃   Link Cable Status:     ✖️     ┃
┃   Cartridge  Status:     ✔     ┃
┃                                ┃
┃   Connect GBC Link Cable       ┃
┃                                ┃
┃                                ┃
┃                                ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
```

#### Boot Screen: Waiting for Link or role change (GB/GBC only)
Flash Cartridge still inserted. Help text will be specific for GB/GBC:
```
┏━━━━━━━━━━━━━━━━━GBC━━━━━━━━━━━━━━━━━━━━┓
┃                                        ┃
┃                                        ┃
┃       Role: Leader                     ┃
┃       Link Cable Status:     ✖️         ┃
┃       Cartridge  Status:     ✔         ┃
┃                                        ┃
┃       Connect GBC Link Cable           ┃
┃       or remove the Cartridge          ┃
┃       to change role                   ┃
┃                                        ┃
┃                                        ┃
┃                                        ┃
┃                                        ┃
┃                                        ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
```

#### Worker Role:
Flash Cartridage removed or multiboot startup: (role switched: Leader -> Worker):
```
┏━━━━━━━━━━━━━━━━━GBC━━━━━━━━━━━━━━━━━━━━┓
┃                                        ┃┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃                                        ┃┃   Role: Worker                 ┃
┃       Role: Worker                     ┃┃   Link Cable Status:     ✖️     ┃
┃       Link Cable Status:     ✔         ┃┃   Cartridge  Status:     ✖️     ┃
┃       Cartridge  Status:     ✔         ┃┃                                ┃
┃                                        ┃┃   Insert GB/GBC Cartridge      ┃
┃       Insert GB/GBC Cartridge          ┃┃                                ┃
┃                                        ┃┃                                ┃
┃                                        ┃┃                                ┃
┃                                        ┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
┃                                        ┃
┃                                        ┃
┃                                        ┃
┃                                        ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
```

Cartridge inserted and Link Cable connection established:
```
┏━━━━━━━━━━━━━━━━━GBC━━━━━━━━━━━━━━━━━━━━┓
┃                                        ┃┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃                                        ┃┃   Role: Worker                 ┃
┃       Role: Worker                     ┃┃   Link Cable Status:     ✔     ┃
┃       Link Cable Status:     ✔         ┃┃   Cartridge  Status:     ✔     ┃
┃       Cartridge  Status:     ✔         ┃┃                                ┃
┃                                        ┃┃      Waiting for Leader        ┃
┃          Waiting for Leader            ┃┃                                ┃
┃                                        ┃┃                                ┃
┃                                        ┃┃                                ┃
┃                                        ┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
┃                                        ┃
┃                                        ┃
┃                                        ┃
┃                                        ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
```
#### Leader Role:
Flash Cartridge is never removed, Link Cable established connection to device with Worker role established:
```
┏━━━━━━━━━━━━━━━━━GBC━━━━━━━━━━━━━━━━━━━━┓
┃                                        ┃┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃                                        ┃┃   Role: Leader                 ┃
┃       Role: Leader                     ┃┃   Link Cable Status:     ✔     ┃
┃       Link Cable Status:     ✔         ┃┃   Cartridge  Status:     ✔     ┃
┃       Cartridge  Status:     ✔         ┃┃                                ┃
┃                                        ┃┃     Press Button to:           ┃
┃          Press Button to:              ┃┃        A: Backup SAVE          ┃
┃             A: Backup SAVE             ┃┃        B: Restore SAVE         ┃
┃             B: Restore SAVE            ┃┃    Start: Backup ROM           ┃
┃                                        ┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
┃                                        ┃
┃                                        ┃
┃                                        ┃
┃                                        ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
```

After choosing mode, show progress:
```
┏━━━━━━━━━━━━━━━━━GBC━━━━━━━━━━━━━━━━━━━━┓
┃                                        ┃┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃                                        ┃┃   Role: Leader                 ┃
┃       Role: Leader                     ┃┃   Link Cable Status:     ✔     ┃
┃       Link Cable Status:     ✔         ┃┃   Cartridge  Status:     ✔     ┃
┃       Cartridge  Status:     ✔         ┃┃                                ┃
┃                                        ┃┃       Backing up SAVE:         ┃
┃           Backing up SAVE:             ┃┃   ┏━━━━━━━━━━━━━━━━━━━━━━━━┓   ┃
┃   ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓    ┃┃   ┃ █████                  ┃   ┃
┃   ┃ ███████                       ┃    ┃┃   ┗━━━━━━━━━━━━━━━━━━━━━━━━┛   ┃
┃   ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛    ┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
┃                                        ┃
┃                                        ┃
┃                                        ┃
┃                                        ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

```

save-manager.gb
=================
~~Single GB/GBC/GBA:~~
- ~~(GB) QR SAVE Backup~~
- ~~(GB) Audio Cable SAVE Backup~~

Link Cable (2x GB/GBC/GBAs):
- (GB) SAVE Backup/Restore *

save-manager.gba
=================
~~Single GBA:~~
- ~~(GBA) QR SAVE Backup~~
- ~~(GBA) Audio Cable SAVE Backup~~

Link Cable (2x GBAs):
- (GB)  SAVE Backup/Restore *
- (GBA) SAVE Backup/Restore

#### ROMs included in others ROMs:
- gb-companion.gb
- gb-companion_mb.gba
- gba-companion_mb.gba

#### ROM inclusion structure:
- save-manager.gb
  - gb-companion.gb
- save-manager.gba
  - gb-companion_mb.gba
    - gb-companion.gb
  - gba-companion_mb.gba

Findings and solutions:
========

- GBA (while in gbc mode), cannot hold data in RAM if cartridge is disconnected (saftey feature?)
  - Solved by putting code in VRAM instead of RAM
- VRAM can only be accessed at certain times in GBC mode
  - Solved by turing the screen off and on rapidly and put main logic in VRAM while HRAM code can be used to render the screen.
- GB screen is too slow to toggle on and off fast
  - Solved by putting code in RAM instead of VRAM
- GBA data written to RAM while cartridge is inserted is retained if a cartridge is ejected and then injected again.
  Seems to be controlled by the hardware switch used for detecting GBC games in cartridge slot. This means that we can
  execute code from RAM if cartridge is inserted and we know cartridge will not be ejected.

#### save-manager.gb codeflow:
- Init steps:
  - Put gb-companion in VRAM and gb-companion_ram in RAM
  - If started from GBA, switch execution to gb-companion (VRAM)
  - If started from GB/GBC switch execution to  gb-companion_ram (RAM)
- Code runs as normal from RAM/VRAM until data transfer should start:
  - Switch execution to RAM which handles data transfer for both 
    leader and worker
  <!-- - If leader, switch back execution to (gb-save-manager) ROM
  - If worker, continue to send data while executing gb-companion in RAM/VRAM -->