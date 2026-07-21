/**
 * @file pmm.h
 * @brief PMM system-level API — power sequencing, LED, fault handling.
 */

#ifndef PMM_H
#define PMM_H

#include "stm32c0xx_hal.h"
#include "bq25895.h"
#include "bq27441.h"
#include <stdbool.h>
#include <stdint.h>

/* ── PMM operating states ──────────────────────────────────────────────── */
typedef enum {
    PMM_STATE_INIT = 0,         /* Power-on, initialising peripherals */
    PMM_STATE_IDLE,             /* BQ25895 present, monitoring only */
    PMM_STATE_CHARGING,         /* Actively charging battery */
    PMM_STATE_FULL,             /* Charge complete */
    PMM_STATE_DISCHARGING,      /* Battery powering 5 V rail */
    PMM_STATE_FAULT,            /* Latched fault condition */
} pmm_state_t;

/* ── PMM snapshot (all key data in one struct) ─────────────────────────── */
typedef struct {
    pmm_state_t     state;
    bq25895_vbus_stat_t  vbus_stat;
    bq25895_chrg_stat_t chrg_stat;
    bool                 power_good;
    bq27441_data_t  batt;
    uint16_t        vbus_mv;          /* ADC-derived VBUS voltage (mV) */
    uint8_t         charger_faults;
    bool            load_switch_on;
    bool            led_on;
} pmm_status_t;

/* ── Public API ────────────────────────────────────────────────────────── */

/**
 * @brief Full system bring-up: clocks, GPIO, I2C, UART, ADC, chip init.
 */
void pmm_init(void);

/**
 * @brief Call once per main-loop iteration.
 *
 * Polls all peripherals, updates internal state machine, drives LED.
 */
void pmm_poll(void);

/**
 * @brief Return latest consolidated status snapshot.
 */
const pmm_status_t *pmm_get_status(void);

/**
 * @brief Enable/disable the 5 V load switch (TPS22917).
 */
void pmm_set_load_switch(bool on);

/**
 * @brief Enable/disable BQ25895 charging.
 */
void pmm_set_charging(bool enable);

/**
 * @brief Set system shutdown (assert SHDN pin).
 */
void pmm_shutdown(void);

/**
 * @brief Print human-readable status over UART.
 */
void pmm_print_status(void);

#endif /* PMM_H */
