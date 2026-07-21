/**
 * @file pmm_config.h
 * @brief PMM-1.0-R1 hardware configuration and pin definitions.
 *
 * Power Management Module — STM32C031C4T6 (LQFP-48, Cortex-M0+, 48 MHz HSI).
 */

#ifndef PMM_CONFIG_H
#define PMM_CONFIG_H

#include "stm32c0xx_hal.h"

/* ── I2C addresses (7-bit) ─────────────────────────────────────────────── */
#define BQ25895_I2C_ADDR        0x6BU
#define BQ27441_I2C_ADDR        0x55U

/* ── I2C bus ───────────────────────────────────────────────────────────── */
#define PMM_I2C                 I2C1
#define PMM_I2C_SCL_PORT        GPIOA
#define PMM_I2C_SCL_PIN         GPIO_PIN_9   /* PA9  — I2C1_SCL */
#define PMM_I2C_SDA_PORT        GPIOA
#define PMM_I2C_SDA_PIN         GPIO_PIN_10  /* PA10 — I2C1_SDA */
#define PMM_I2C_SPEED           400000U      /* 400 kHz fast-mode */

/* ── UART (debug) ─────────────────────────────────────────────────────── */
#define PMM_UART                USART2
#define PMM_UART_TX_PORT        GPIOA
#define PMM_UART_TX_PIN         GPIO_PIN_2   /* PA2 — USART2_TX */
#define PMM_UART_RX_PORT        GPIOA
#define PMM_UART_RX_PIN         GPIO_PIN_3   /* PA3 — USART2_RX */
#define PMM_UART_BAUD           115200U

/* ── ADC ───────────────────────────────────────────────────────────────── */
#define PMM_ADC                 ADC1
#define PMM_ADC_CH              ADC_CHANNEL_8   /* PA8 — ADC1_IN8 */
#define PMM_ADC_PORT            GPIOA
#define PMM_ADC_PIN             GPIO_PIN_8

/* ── GPIO outputs ──────────────────────────────────────────────────────── */
#define LOAD_SW_PORT            GPIOA
#define LOAD_SW_PIN             GPIO_PIN_0   /* PA0 — TPS22917 ON (active high) */

#define CHG_CE_PORT             GPIOA
#define CHG_CE_PIN              GPIO_PIN_4   /* PA4 — BQ25895 ~CE (active low) */

#define LED_PORT                GPIOA
#define LED_PIN                 GPIO_PIN_6   /* PA6 — D3 status LED (active high) */

#define PMM_INT_PORT            GPIOA
#define PMM_INT_PIN             GPIO_PIN_7   /* PA7 — PMM_INT (to host) */

#define SHDN_PORT               GPIOA
#define SHDN_PIN                GPIO_PIN_11  /* PA11 — SHDN control */

#define FORCE_ON_PORT           GPIOA
#define FORCE_ON_PIN            GPIO_PIN_12  /* PA12 — FORCE_ON control */

/* ── GPIO inputs ───────────────────────────────────────────────────────── */
#define BQ27441_INT_PORT        GPIOA
#define BQ27441_INT_PIN         GPIO_PIN_1   /* PA1 — BQ27441 GPOUT (active low) */

#define BQ25895_INT_PORT        GPIOA
#define BQ25895_INT_PIN         GPIO_PIN_5   /* PA5 — BQ25895 INT (active low) */

/* ── Clock configuration ───────────────────────────────────────────────── */
#define HSI_VALUE_HZ            48000000UL
#define SYSTEM_CLOCK_HZ         48000000UL

/* ── Timing constants (ms) ─────────────────────────────────────────────── */
#define LED_BLINK_PERIOD_MS     500U
#define POLL_INTERVAL_MS        1000U
#define STATUS_PRINT_MS         5000U
#define DEBOUNCE_MS             50U

/* ── Current sense resistor (10 mΩ) ───────────────────────────────────── */
#define SHUNT_RESISTANCE_MOHM   10U

#endif /* PMM_CONFIG_H */
