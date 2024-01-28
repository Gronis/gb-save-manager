## 2-system Menu (GB and GBA works the same or very similar)

### Screens:

#### Boot Screen (GBA only)
The user have to choose between GB and GBA mode:
```
Role: Leader



  Press Button to use:

      A:  Enter GB/GBC Cartridge Mode

      B:  Enter GBA Cartridge Mode

```
#### Cartridge Mode Selected (GBA only)
Help text will be specific on GBA since GBA will send Worker app with Multiboot:
```
Role: Leader
Link Cable Status: ✖️
Cartridge  Status: ✔

  Connect this device using a GBC Link Cable
  to another GBA that will be used as Worker
```

#### Boot Screen (GB/GBC only)
Flash Cartridge still inserted. Help text will be specific for GB/GBC:
```
Role: Leader
Link Cable Status: ✖️
Cartridge  Status: ✔

  Connect this device using a
  GBC Link Cable or remove the
  Flash Cartridge to change role
```

#### Worker Role:
Flash Cartridage removed or multiboot starup: (role switched: Leader -> Worker):
```
Role: Worker
Link Cable Status: ✖️
Cartridge  Status: ✖️
 
  Insert GB/GBC Cartridge
```

Flash Cartridge inserted and Link Cable connection established:
```
Role: Worker
Link Cable Status: ✔
Cartridge  Status: ✔


```
#### Leader Role:
Flash Cartridge is never removed, Link Cable established connection to device with Worker role established:
```
Role: Leader
Link Cable Status: ✔
Cartridge  Status: ✔

  Press Button to:

      A:  Backup SAVE from 
          Worker Cartridge

      B:  Restore SAVE to
          Worker Cartridge

  Start:  Backup ROM from
          Worker Cartridge  (Everdrive GBA only)
```

After choosing mode, show progress:
```
Role: Leader
Link Cable Status: ✔
Cartridge  Status: ✔

  Backing up SAVE:
  ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
  ┃ ███████                        ┃ 
  ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
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
