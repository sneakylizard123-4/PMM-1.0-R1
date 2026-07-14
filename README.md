# PMM-1.0-R1 (Power Management Module)

A USB-powered battery management system (BMS) board with fuel gauge, designed for Li-ion/LiPo battery charging and state-of-charge monitoring.

## Features

- **USB Type-C Charging** - 5V input via USB-C connector
- **Battery Charger IC** - TI BQ25895 (2A buck charger)
- **Fuel Gauge** - TI BQ27441-G1 for state-of-charge (SoC) monitoring
- **5V Boost Regulator** - TPS61089 for output boost
- **3.3V LDO** - TLV75733 for MCU power
- **Load Switch** - TPS22917 for protected output
- **Current Sensing** - 0.01Ω shunt resistor for current measurement
- **Temperature Monitoring** - NTC thermistor for battery temperature

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

## Specifications

| Parameter | Value |
|-----------|-------|
| Input Voltage | 5V USB |
| Output Voltage | 5V (boosted) |
| MCU Voltage | 3.3V |
| Charging Current | Up to 2A |
| Board Dimensions | ~50mm x ~30mm |

## Component Summary

| Type | Count |
|------|-------|
| ICs | 7 |
| Capacitors | 29 |
| Resistors | 22 |
| LEDs | 3 |
| Ferrite Beads | 2 |
| Thermistor | 1 |
| Test Points | 2 |
| **Total Components** | **66** |

## Key ICs

| Reference | Part | Description |
|-----------|------|-------------|
| U1 | TPS61089 | 5V synchronous boost converter |
| U2 | BQ25895 | USB Type-C buck charger |
| U3 | BQ27441-G1 | Battery fuel gauge (SoC) |
| U4 | STM32C031C4 | 32-bit ARM Cortex-M0+ MCU |
| U5 | TLV75733PDBV | 3.3V LDO regulator |
| U6 | TPS22917 | Load switch |

## I2C Devices

The following devices communicate via I2C:
- BQ27441 Fuel Gauge (SoC monitoring)
- STM32 MCU
- Other I2C peripherals

## Files

```
PMM-1.0-R1/
├── README.md              # This file
├── .gitignore
├── datasheets/           # Component datasheets
│   ├── bq25895.pdf
│   ├── bq27441-g1.pdf
│   ├── stm32c031c4.pdf
│   ├── tlv757p.pdf
│   ├── tps22917.pdf
│   └── tps61089.pdf
└── kicad/               # KiCad project files
    ├── PMM-1.0-R1.kicad_pro
    ├── PMM-1.0-R1.kicad_sch
    ├── PMM-1.0-R1.kicad_pcb
    ├── PMM-1.0-R1.kicad_prl
    ├── bq25895.kicad_sch
    ├── bq27441.kicad_sch
    ├── stm32.kicad_sch
    ├── tlv75733.kicad_sch
    ├── tps22917.kicad_sch
    ├── tps61089.kicad_sch
    └── fp-lib-table
```

## Design Notes

- Hierarchical schematic design with separate sheets for each major IC
- 4-layer PCB stack-up recommended for optimal EMI performance
- Thermal considerations for charging and boost regulator circuits

## License

Open source hardware - see project repository for details.

## Development

This project uses [kicad-happy](https://github.com/aklofas/kicad-happy) for automated design review:

```bash
# Analyze schematic
python3 ~/.claude/plugins/cache/kicad-happy/kicad-happy/2.0.0/skills/kicad/scripts/analyze_schematic.py kicad/PMM-1.0-R1.kicad_sch --output analysis.json

# Analyze PCB
python3 ~/.claude/plugins/cache/kicad-happy/kicad-happy/2.0.0/skills/kicad/scripts/analyze_pcb.py kicad/PMM-1.0-R1.kicad_pcb --output pcb_analysis.json
```