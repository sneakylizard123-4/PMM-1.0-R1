# PMM-1.0-R1 (Power Management Module)

A USB-C battery management module with charging, fuel gauging, and a switched 5V output.

## Custom Features

- BQ25895 2A buck charger with I2C control
- BQ27441-G1 fuel gauge
- TPS61089 boost converter for 5V output
- TPS22917 load
- TLV75733 3.3V LDO
- STM32C031C4T6 MCU
- 4-layer PCB, 50x30mm

## PCB Design

this board is designed to fit on a breadboard without much hassle
it is a 4-layer pcb with an inner ground layer

![PCB Top](images/pcb-top.png)
![PCB Bottom](images/pcb-bottom.png)
![3D View](images/pcb-3d.png)

## Firmware

PlatformIO project using internal 48MHz oscillator (no crystal).

- I2C1 (PA9/PA10): BQ25895 charger at 0x6B, BQ27441 fuel gauge at 0x55
- USART2 (PA2/PA3): 115200 debug/status output
- ADC1 (PA8): VBUS monitoring
- GPIO: load switch (PA0), charger CE (PA4), LED (PA6), SHDN (PA11), FORCE_ON (PA12)

Flash with PlatformIO:

```
pio run -t upload
```

## BOM (Bill of Materials)

| Ref | Value | LCSC # | MPN | Manufacturer | Footprint | Qty | Unit $ | Ext $ | Link |
|-----|-------|--------|-----|--------------|-----------|-----|--------|-------|------|
| C1-C3,C8,C10 | 47uF 1210 16V X5R | C6783889 | 1210X476M160CT | Walsin | 1210 | 5 | 0.0900 | 0.4500 | [LCSC](https://www.lcsc.com/product-detail/C6783889.html) |
| C4,C7,C14,C27,C29 | 100nF 0402 25V X7R | C56392 | 0402B104K250NT | FH(Guangdong Fenghua) | 0402 | 5 | 0.0115 | 0.0575 | [LCSC](https://www.lcsc.com/product-detail/C56392.html) |
| C5 | 100nF 0805 50V X7R | C83055 | 0805B104K500CT | Walsin | 0805 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C83055.html) |
| C6,C22,C23,C28 | 10uF 0603 10V X5R | C19702 | CL10A106KP8NNNC | Samsung | 0603 | 4 | 0.0065 | 0.0260 | [LCSC](https://www.lcsc.com/product-detail/C19702.html) |
| C9 | 24pF 0402 C0G | C2242874 | C0402C240J4GACTU | KEMET | 0402 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C2242874.html) |
| C11 | 560pF 0402 C0G 50V | C3895255 | C0402C0G500-561FNP | Venkel | 0402 | 1 | 0.0313 | 0.0313 | [LCSC](https://www.lcsc.com/product-detail/C3895255.html) |
| C12,C19 | 100nF 0805 50V X7R | C83055 | 0805B104K500CT | Walsin | 0805 | 2 | 0.0100 | 0.0200 | [LCSC](https://www.lcsc.com/product-detail/C83055.html) |
| C13,C18 | 10uF 0805 10V X7R | C396720 | 0805B106K100CT | Walsin | 0805 | 2 | 0.0950 | 0.1900 | [LCSC](https://www.lcsc.com/product-detail/C396720.html) |
| C15 | 47uF 0805 10V X5R | C6119904 | CGA0805X5R476M100MT | HRE | 0805 | 1 | 0.1000 | 0.1000 | [LCSC](https://www.lcsc.com/product-detail/C6119904.html) |
| C16 | 1uF 0805 16V X7R | C2991444 | 0805B105K160CT-C | Walsin | 0805 | 1 | 0.0230 | 0.0230 | [LCSC](https://www.lcsc.com/product-detail/C2991444.html) |
| C17 | 0.47uF 0402 10V X5R | C47339 | CL05A474KP5NNNC | Samsung | 0402 | 1 | 0.0200 | 0.0200 | [LCSC](https://www.lcsc.com/product-detail/C47339.html) |
| C20 | 47nF 0603 25V X7R | C3881195 | C0603C473J3RAC7867 | KEMET | 0603 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C3881195.html) |
| C21 | 4.7uF 0402 6.3V X5R | C340648 | 0402X475M6R3CT | Walsin | 0402 | 1 | 0.0240 | 0.0240 | [LCSC](https://www.lcsc.com/product-detail/C340648.html) |
| C24,C25 | 1uF 0603 16V X7R | C106248 | CC0603KRX7R7BB105 | YAGEO | 0603 | 2 | 0.0310 | 0.0620 | [LCSC](https://www.lcsc.com/product-detail/C106248.html) |
| C26 | 100nF 0603 50V X7R | C14663 | CC0603KRX7R9BB104 | YAGEO | 0603 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C14663.html) |
| D1-D3 | LED 0603 emerald green | C7429914 | NCD0603G3 | NATIONSTAR | 0603 | 3 | 0.0080 | 0.0240 | [LCSC](https://www.lcsc.com/product-detail/C7429914.html) |
| L1 | 470nH shielded power inductor | C18221164 | XFL4015-471MEC | Coilcraft | SMD 4x4mm | 1 | 1.5000 | 1.5000 | [LCSC](https://www.lcsc.com/product-detail/C18221164.html) |
| L2 | 2.2uH shielded power inductor 8A | C122469 | XFL4020-222MEC | Coilcraft | SMD 4x4mm | 1 | 1.5000 | 1.5000 | [LCSC](https://www.lcsc.com/product-detail/C122469.html) |
| R1,R16,R19,R20 | 10k 0603 1% | C98220 | RC0603FR-0710KL | YAGEO | 0603 | 4 | 0.0100 | 0.0400 | [LCSC](https://www.lcsc.com/product-detail/C98220.html) |
| R2 | 309k 0402 1% | C171694 | RC0402FR-07309KL | YAGEO | 0402 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C171694.html) |
| R3 | 133k 0402 1% | C3015855 | SCR0402F133K | VO | 0402 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C3015855.html) |
| R4 | 28.7k 0402 1% | C185433 | RC0402FR-0728K7L | YAGEO | 0402 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C185433.html) |
| R5 | 240k 0603 1% | C137765 | RC0603FR-07240KL | YAGEO | 0603 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C137765.html) |
| R6 | 76.8k 0402 1% | C185396 | RC0402FR-0776K8L | YAGEO | 0402 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C185396.html) |
| R7 | 100k 0402 1% | C60491 | RC0402FR-07100KL | YAGEO | 0402 | 1 | 0.0060 | 0.0060 | [LCSC](https://www.lcsc.com/product-detail/C60491.html) |
| R8 | 100k 0603 1% | C14675 | RC0603FR-07100KL | YAGEO | 0603 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C14675.html) |
| R9 | 0.010 ohm 1% 2512 3W shunt | C520398 | LCSR2512FR010Q9L | RESI | 2512 | 1 | 0.1000 | 0.1000 | [LCSC](https://www.lcsc.com/product-detail/C520398.html) |
| R10-R14 | 10k 0402 1% | C60490 | RC0402FR-0710KL | YAGEO | 0402 | 5 | 0.0100 | 0.0500 | [LCSC](https://www.lcsc.com/product-detail/C60490.html) |
| R15 | 130 ohm 0805 1% | C273821 | RC0805FR-07130RL | YAGEO | 0805 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C273821.html) |
| R17 | 5.23k 0402 1% | C477746 | RC0402FR-075K23L | YAGEO | 0402 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C477746.html) |
| R18 | 30.1k 0402 1% | C138009 | RC0402FR-0730K1L | YAGEO | 0402 | 1 | 0.0100 | 0.0100 | [LCSC](https://www.lcsc.com/product-detail/C138009.html) |
| R21,R22 | 4.7k 0402 1% | C105871 | RC0402FR-074K7L | YAGEO | 0402 | 2 | 0.0100 | 0.0200 | [LCSC](https://www.lcsc.com/product-detail/C105871.html) |
| TH1 | 10k NTC thermistor 0402 | C77131 | NCP15XH103F03RC | muRata | 0402 | 1 | 0.0266 | 0.0266 | [LCSC](https://www.lcsc.com/product-detail/C77131.html) |
| U1 | TPS61089RNRR boost converter | C165129 | TPS61089RNRR | Texas Instruments | VQFN-11 | 1 | 0.5800 | 0.5800 | [LCSC](https://www.lcsc.com/product-detail/C165129.html) |
| U2 | TPS22917DBVR load switch | C2681320 | TPS22917DBVR | Texas Instruments | SOT-23-6 | 1 | 0.4600 | 0.4600 | [LCSC](https://www.lcsc.com/product-detail/C2681320.html) |
| U3 | BQ27441DRZR-G1B fuel gauge | C473374 | BQ27441DRZR-G1B | Texas Instruments | SON-12 | 1 | 2.7600 | 2.7600 | [LCSC](https://www.lcsc.com/product-detail/C473374.html) |
| U4 | BQ25895RTWR charger | C80200 | BQ25895RTWR | Texas Instruments | WQFN-24 | 1 | 2.1700 | 2.1700 | [LCSC](https://www.lcsc.com/product-detail/C80200.html) |
| U5 | TLV75733PDBVR 3.3V LDO | C485517 | TLV75733PDBVR | Texas Instruments | SOT-23-5 | 1 | 0.1950 | 0.1950 | [LCSC](https://www.lcsc.com/product-detail/C485517.html) |
| U6 | STM32C031C4T6 MCU | C5456207 | STM32C031C4T6 | STMicroelectronics | LQFP-48 | 1 | 1.6700 | 1.6700 | [LCSC](https://www.lcsc.com/product-detail/C5456207.html) |
| PCB | Bare PCB 50x30mm 4-layer | - | - | - | - | 1 | 2.5000 | 2.5000 | - |
| **Total** | - | - | - | - | - | **64** | - | **14.7354** | - |

Full BOM CSV: [BOM.csv](BOM.csv)

## Production

This board is designed for a standard 4-layer PCB fab:
- 4 layers
- HASL Lead Free
- 1.6mm board