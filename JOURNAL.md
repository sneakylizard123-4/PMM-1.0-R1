---
title: "PMM-1.0-R1"
author: pn2222a
description: "Power Management Module — USB-C charging, battery fuel gauge, 5V boost output"
created_at: "2026-07-05T00:00:00Z"
---



# July 5: first commit

started the KiCad project with the root schematic.
added datasheets for all the key ICs: BQ25895, BQ27441-G1, STM32C031C4, TPS22917, TPS61089.
ran TPS61089 WebBench simulations and saved the waveforms and BOM.
![sim](datasheets/simulations/TPS61089/amCharts.png)

**Total time spent: 4 hours**

# July 7: 3.3V regulator

worked on the 3.3V regulator section of the schematic.
started laying out the TLV75733 LDO subcircuit.

**Total time spent: 2 hours**

# July 9: sub-sheets and regulator fixes

broke the schematic into hierarchical sub-sheets for each major IC:
BQ25895 charger, BQ27441 fuel gauge, TPS61089 boost, TPS22917 load switch, TLV75733 LDO, STM32 MCU.
fixed the 3.3V regulator circuit and cleaned up the sub-sheet connections.
reorganized gitignore.

**Total time spent: 3 hours**

# July 13: finished schematics

completed all schematic sub-sheets and net connections.
added the STM32 sub-sheet with full MCU pin mapping.
created a custom PMM symbol and footprint in the Parts library.
![sch](images/SCH.png)

**Total time spent: 5 hours**

# July 14: PCB routing

started PCB layout and routed most of the board.
added the TPS61089 datasheet (tlv757p.pdf).
finished the schematic and did multiple routing passes.
added the README with full project documentation and power architecture diagram.

**Total time spent: 6 hours**

# July 16: finished PCB

finished the 4-layer PCB layout.
completed BQ25895 routing and all remaining traces.
added PMM symbol and footprint to Parts library.

**Total time spent: 5 hours**

# July 17: silkscreen and cleanup

added silkscreen art — empire and linux penguin logos.
reorganized the Parts library structure (renamed logo/ to logo.pretty/).
moved the schematic sub-sheets into a hierarchical backup, cleaned up the main project.
updated the README.

**Total time spent: 3 hours**

# July 21: firmware

created PlatformIO firmware for the PMM.
MCU is STM32C031C4T6 @ 48 MHz HSI (no external crystal).
I2C1 on PA9/PA10: BQ25895 charger (0x6B) and BQ27441 fuel gauge (0x55).
USART2 on PA2/PA3 for 115200 debug. ADC1 on PA8 for VBUS monitoring.
GPIO: PA0 load switch, PA4 charger CE, PA6 LED, PA11 SHDN, PA12 FORCE_ON.

wrote BQ25895 and BQ27441 I2C drivers, system init, state machine, LED blinker,
periodic UART status print. Had to install STM32CubeC0 from ST GitHub since
PlatformIO doesn't ship it yet. Fixed build errors: wrong I2C AF, no APB prescaler
on C0, ADC sample time, extern declarations.

RAM: 4.5% (552 bytes). Flash: 62.3% (20 KB).

**Total time spent: 4 hours**
