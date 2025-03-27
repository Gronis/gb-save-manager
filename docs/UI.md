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
┃   Connect GBC Link Cable.      ┃
┃   Then start the other GBA     ┃
┃   WITHOUT a game cartridge     ┃
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