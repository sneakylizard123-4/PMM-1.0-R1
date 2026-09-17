---
title: PMM-1.0-R1
author: sneakylizard123-4
description: "Power Management Module - USB-C charging, battery fuel gauge, 5V boost output"
created_at: 2026-07-05
---



# July 5: first commit

started pmm project
needed an easy power solution

it has a dedicated fuel gauge and boost converter

used ti web bench to make sim
![sim](images/sim-tps61089.png)

**Total Time Spent: 4 hours**

# July 7: 3.3V regulator

added a 3.3v regulator
used tlv75733 ldo
it doesnt need to be too complex

![tlv75733](images/PMM-1.0-R1-TLV75733.svg)

**Total Time Spent: 2 hours**

# July 9: sub-sheets and regulator fixes

split into subsheets for more clean schematic
each sheet has its own dedicated thing
fixed the 3.3v regulator.
![root-sch](images/PMM-1.0-R1.svg)

**Total Time Spent: 3 hours**

# July 13: finished schematics

connected all the subsheets together
wired up the stm32

drew a custom pmm schematic and footprint to make wiring easier, it has castellated edges like a pi pico
![stm32](images/PMM-1.0-R1-STM32.svg)

**Total Time Spent: 5 hours**

# July 14: PCB routing

started routing pcb, power is hard
added tlv757 datasheet
![pcb](images/pcb-top.png)

**Total Time Spent: 6 hours**

# July 16: finished PCB

finished routing pcb, high power areas were tough
routed the remaining charger traces.
![pcb](images/pcb-top.png)

**Total Time Spent: 5 hours**

# July 17: silkscreen and cleanup

silkscreen art: empire logo and penguin.
cleaned up parts library
![pcb-bottom](images/pcb-bottom.png)

**Total Time Spent: 3 hours**

# July 21: firmware

using platformio for firmware
using internal oscillator
using i2c and usart
wrote firmware in firmware/
still have lots of headroom in flash and ram
![bq25895](images/PMM-1.0-R1-BQ25895.svg)

**Total Time Spent: 4 hours**
