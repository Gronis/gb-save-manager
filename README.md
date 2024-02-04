## 2-system Menu (GB and GBA works the same or very similar)

### Screens:

#### Boot Screen: Choose Cartridge Mode (GBA only)
The user have to choose between GB and GBA mode:
```
┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃                                ┃
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
┃                                ┃
┃   Role: Leader                 ┃
┃   Link Cable Status:     ✖️     ┃
┃   Cartridge  Status:     ✔     ┃
┃                                ┃
┃   Connect another GBA using    ┃
┃   a GBC Link Cable which will  ┃
┃   be used as Worker            ┃
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
┃     Connect this device using a        ┃
┃     GBC Link Cable or remove the       ┃
┃     Flash Cartridge to change role     ┃
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
┏━━━━━━━━━━━━━━━━━GBC━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃                                        ┃┃                                ┃
┃                                        ┃┃   Role: Worker                 ┃
┃       Role: Worker                     ┃┃   Link Cable Status:     ✖️     ┃
┃       Link Cable Status:     ✔         ┃┃   Cartridge  Status:     ✖️     ┃
┃       Cartridge  Status:     ✔         ┃┃                                ┃
┃                                        ┃┃       Insert GB/GBC            ┃
┃           Insert GB/GBC                ┃┃       Cartridge                ┃
┃           Cartridge                    ┃┃                                ┃
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
┏━━━━━━━━━━━━━━━━━GBC━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃                                        ┃┃                                ┃
┃                                        ┃┃   Role: Worker                 ┃
┃       Role: Worker                     ┃┃   Link Cable Status:     ✔     ┃
┃       Link Cable Status:     ✔         ┃┃   Cartridge  Status:     ✔     ┃
┃       Cartridge  Status:     ✔         ┃┃                                ┃
┃                                        ┃┃       Screen will flash        ┃
┃           Screen will flash            ┃┃       while working!           ┃
┃           while working!               ┃┃                                ┃
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
┏━━━━━━━━━━━━━━━━━GBC━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃                                        ┃┃                                ┃
┃                                        ┃┃   Role: Leader                 ┃
┃       Role: Leader                     ┃┃   Link Cable Status:     ✔     ┃
┃       Link Cable Status:     ✔         ┃┃   Cartridge  Status:     ✔     ┃
┃       Cartridge  Status:     ✔         ┃┃                                ┃
┃                                        ┃┃     Press Button to:           ┃
┃          Press Button to:              ┃┃        A: Backup SAVE          ┃
┃                                        ┃┃        B: Restore SAVE         ┃
┃          A: Backup SAVE from           ┃┃    Start: Backup ROM           ┃
┃             Worker Cartridge           ┃┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
┃                                        ┃
┃          B: Restore SAVE to            ┃
┃             Worker Cartridge           ┃
┃                                        ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
```

After choosing mode, show progress:
```
┏━━━━━━━━━━━━━━━━━GBC━━━━━━━━━━━━━━━━━━━━┓┏━━━━━━━━━━GBA━ZOOMED━━━━━━━━━━━━┓
┃                                        ┃┃                                ┃
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

ROMs included in others ROMs:
- gb-companion.gb
- gb-companion_mb.gba
- gba-companion_mb.gba

ROM inclusion structure:
- save-manager.gb
  - gb-companion.gb
- save-manager.gba
  - gb-companion_mb.gba
    - gb-companion.gb
  - gba-companion_mb.gba
