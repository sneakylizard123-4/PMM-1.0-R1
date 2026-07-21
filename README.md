# PMM-1.0-R1 (Power Management Module)

A USB-powered battery management system (BMS) board with fuel gauge, designed for Li-ion/LiPo battery charging and state-of-charge monitoring.

---

## Key Features

- **USB Type-C Charging** - 5V input via USB-C connector
- **Battery Charger IC** - TI BQ25895 (2A buck charger)
- **Fuel Gauge** - TI BQ27441-G1 for state-of-charge (SoC) monitoring
- **5V Boost Regulator** - TPS61089 for output boost
- **3.3V LDO** - TLV75733 for MCU power
- **Load Switch** - TPS22917 for protected output
- **Current Sensing** - 0.01Ω shunt resistor for current measurement
- **Temperature Monitoring** - NTC thermistor for battery temperature
- **MCU** - STM32C031C4T6 (Cortex-M0+, 48MHz)
- **Compact 4-layer PCB**

---

## Power Architecture

```
USB VBUS (5V) 
    │
    ├──► BQ25895 (Charger IC)
    │         │
    │         ▼
    │    Battery (Li-ion)
    │         │
    │         ▼
    │    TPS61089 (Boost) ───► 5V Output
    │         │
    │         ▼
    │    TLV75733 (LDO) ───► 3.3V MCU
    │
    └──► BQ27441 (Fuel Gauge)
```

---

## Specifications

| Parameter | Value |
|-----------|-------|
| Input Voltage | 5V USB |
| Output Voltage | 5V (boosted) |
| MCU Voltage | 3.3V |
| Charging Current | Up to 2A |
| Board Dimensions | ~50mm x ~30mm |
| Layers | 4 |

---

## Bill of Materials

| References | Qty | Value | Footprint |
|------------|-----|-------|-----------|
| U1 | 1 | TPS61089 | VQFN-11 |
| U2 | 1 | TPS22917DBV | SOT-23-6 |
| U3 | 1 | BQ27441-G1 | DSON-12 |
| U4 | 1 | BQ25895RTW | QFN-24 |
| U5 | 1 | TLV75733PDBV | SOT-23-5 |
| U6 | 1 | STM32C031C4T6 | LQFP-48 |
| C1-C3,C8,C10 | 5 | 47uF | 1210 |
| C4,C7,C14,C27,C29 | 5 | 100nF | 0402 |
| C5 | 1 | 0.1uF | 0805 |
| C6,C22,C23,C28 | 4 | 10uF | 0603 |
| C9 | 1 | 24pF | 0402 |
| C11 | 1 | 560pF | 0402 |
| C12,C19 | 2 | 100nF | 0805 |
| C13,C18 | 2 | 10uF | 0805 |
| C15 | 1 | 47uF | 0805 |
| C16 | 1 | 1uF | 0805 |
| C17 | 1 | 0.47uF | 0402 |
| C20 | 1 | 47nF | 0603 |
| C21 | 1 | 4.7uF | 0402 |
| C24,C25 | 2 | 1uF | 0603 |
| C26 | 1 | 100nF | 0603 |
| D1-D3 | 3 | LED | 0603 |
| L1 | 1 | 470nH Coilcraft XFL4015-471MEC | XxL4020 |
| L2 | 1 | 2.2uH Coilcraft XAL4020-222MEC | XxL4020 |
| R1,R16,R19,R20 | 4 | 10k | 0603 |
| R2 | 1 | 309k | 0402 |
| R3 | 1 | 133k | 0402 |
| R4 | 1 | 28.7k | 0402 |
| R5 | 1 | 240k | 0603 |
| R6 | 1 | 76.8k | 0402 |
| R7 | 1 | 100k | 0402 |
| R8 | 1 | 100k | 0603 |
| R9 | 1 | 0.010 1% | 2512 |
| R10-R14 | 5 | 10k | 0402 |
| R15 | 1 | 130 1% | 0805 |
| R17 | 1 | 5.23k | 0402 |
| R18 | 1 | 30.1k | 0402 |
| R21,R22 | 2 | 4.7k | 0402 |
| TH1 | 1 | 10k NTC | 0402 |
| TP1-TP4 | 4 | TestPoint | Pad 1.5x1.5mm |

*Full BOM with manufacturer part numbers available in [BOM.csv](BOM.csv)*

---

## Firmware

PlatformIO project targeting STM32C031C4T6 (Cortex-M0+, 48MHz HSI).

- I2C1 (PA9/PA10): BQ25895 charger (0x6B) + BQ27441 fuel gauge (0x55)
- USART2 (PA2/PA3): 115200 debug
- ADC1 (PA8): VBUS voltage monitoring
- GPIO: load switch, charger CE, LED, SHDN, FORCE_ON

Build: RAM 4.5%, Flash 62.3%

---

## Images

### PCB Layout
![PCB Top](images/pcb-top.png)

### PCB Bottom (Silkscreen)
![PCB Bottom](images/pcb-bottom.png)

### Schematic
![Root Schematic](images/PMM-1.0-R1.svg)

---

*Full BOM available in [BOM.csv](BOM.csv)*
