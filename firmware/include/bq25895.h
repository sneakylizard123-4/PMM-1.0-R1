/**
 * @file bq25895.h
 * @brief BQ25895 USB buck-charger I2C driver (I2C addr 0x6B).
 *
 * Register map based on TI SLUUBC4 datasheet.
 */

#ifndef BQ25895_H
#define BQ25895_H

#include "stm32c0xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

/* ── I2C address ───────────────────────────────────────────────────────── */
#define BQ25895_ADDR_W  ((BQ25895_I2C_ADDR << 1) | 0)   /* 0xD6 */
#define BQ25895_ADDR_R  ((BQ25895_I2C_ADDR << 1) | 1)   /* 0xD7 */

/* ── Register addresses ────────────────────────────────────────────────── */
typedef enum {
    BQ25895_REG_00           = 0x00,
    BQ25895_REG_01           = 0x01,
    BQ25895_REG_02           = 0x02,
    BQ25895_REG_03           = 0x03,
    BQ25895_REG_04           = 0x04,
    BQ25895_REG_05           = 0x05,
    BQ25895_REG_06           = 0x06,
    BQ25895_REG_07           = 0x07,
    BQ25895_REG_08           = 0x08,
    BQ25895_REG_09           = 0x09,
    BQ25895_REG_0A           = 0x0A,
    BQ25895_REG_0B           = 0x0B,
    BQ25895_REG_0C           = 0x0C,
    BQ25895_REG_0D           = 0x0D,
    BQ25895_REG_0E           = 0x0E,
    BQ25895_REG_0F           = 0x0F,
    BQ25895_REG_10           = 0x10,
    BQ25895_REG_11           = 0x11,
    BQ25895_REG_12           = 0x12,
    BQ25895_REG_13           = 0x13,
    BQ25895_REG_14           = 0x14,
} bq25895_reg_t;

/* ── REG00: Input Source Control ───────────────────────────────────────── */
#define BQ25895_REG00_EN_HIZ           BIT(7)
#define BQ25895_REG00_EN_ILIM          BIT(6)
#define BQ25895_REG00_IINLIM_MASK      0x3FU  /* bits [5:0] */
#define BQ25895_REG00_IINLIM_SHIFT     0
#define BQ25895_IINLIM_STEP_MA         50U
#define BQ25895_IINLIM_BASE_MA         100U

/* ── REG01: Input Voltage / VINDPM ─────────────────────────────────────── */
#define BQ25895_REG01_VINDPM_MASK      0x7FU  /* bits [6:0] mV offset = 3900 */
#define BQ25895_REG01_VDPS             BIT(7)

/* ── REG02: System Voltage / Watchdog ──────────────────────────────────── */
#define BQ25895_REG02_EN_LWPWR         BIT(7)
#define BQ25895_REG02_WDT_MASK         0x60U  /* bits [6:5] */
#define BQ25895_REG02_WDT_SHIFT        5
#define BQ25895_REG02_SYSMIN_MASK      0x0EU  /* bits [3:1] */

/* ── REG03: Charge Control ─────────────────────────────────────────────── */
#define BQ25895_REG03_PFM_DIS          BIT(7)
#define BQ25895_REG03_DPDM_EN          BIT(6)
#define BQ25895_REG03_CONV_ROAT        BIT(5)
#define BQ25895_REG03_BAT_LOAD_EN      BIT(4)
#define BQ25895_REG03_BOOST_LIM_MASK   0x0EU  /* bits [3:1] */

/* ── REG04: Charge Current ─────────────────────────────────────────────── */
#define BQ25895_REG04_ICHG_MASK        0x7FU  /* bits [6:0] */
#define BQ25895_REG04_ICHG_STEP_MA     64U
#define BQ25895_REG04_ICHG_BASE_MA     0U

/* ── REG05: Precharge / Termination ────────────────────────────────────── */
#define BQ25895_REG05_IPRECHG_MASK     0xF0U  /* bits [7:4] */
#define BQ25895_REG05_IPRECHG_SHIFT    4
#define BQ25895_REG05_ITERM_MASK       0x0FU  /* bits [3:0] */
#define BQ25895_ITERM_STEP_MA          64U
#define BQ25895_ITERM_BASE_MA          64U

/* ── REG06: Charge Voltage ─────────────────────────────────────────────── */
#define BQ25895_REG06_VREG_MASK        0xFEU  /* bits [7:1] */
#define BQ25895_REG06_VREG_SHIFT       1
#define BQ25895_REG06_VREG_STEP_MV     16U
#define BQ25895_REG06_VREG_BASE_MV     3840U
#define BQ25895_REG06_BATLOWV          BIT(0)

/* ── REG07: Misc Operation ─────────────────────────────────────────────── */
#define BQ25895_REG07_PFM_OTG_DIS      BIT(7)
#define BQ25895_REG07_PFM_DCM_DIS      BIT(6)
#define BQ25895_REG07_WDT_MASK         0x30U  /* bits [5:4] — same watchdog as REG02 */

/* ── REG08: System Status (read-only) ──────────────────────────────────── */
#define BQ25895_REG08_VBUS_GD          BIT(7)
#define BQ25895_REG08_VBUS_STAT_MASK   0x60U  /* bits [6:5] */
#define BQ25895_REG08_VBUS_STAT_SHIFT  5
#define BQ25895_REG08_CHRG_STAT_MASK   0x18U  /* bits [4:3] */
#define BQ25895_REG08_CHRG_STAT_SHIFT  3
#define BQ25895_REG08_PG_STAT          BIT(2)
#define BQ25895_REG08_THERM_STAT       BIT(1)
#define BQ25895_REG08_OTG_STAT         BIT(0)

/* ── REG09: Fault (read-only) ──────────────────────────────────────────── */
#define BQ25895_REG09_WATCHDOG         BIT(7)
#define BQ25895_REG09_OTG_PLUG         BIT(6)
#define BQ25895_REG09_OTG_UVP          BIT(5)
#define BQ25895_REG09_ALRM_STAT        BIT(4)  /* OTG faults */
#define BQ25895_REG09_CHRG_FAULT_MASK  0x30U
#define BQ25895_REG09_BAT_FAULT        BIT(3)
#define BQ25895_REG09_NTC_FAULT_MASK   0x07U

/* ── REG0E: Boost Status (read-only) ───────────────────────────────────── */
#define BQ25895_REG0E_VBUS_STAT_MASK   0xE0U  /* bits [7:5] */

/* ── REG0F: Charger Mask / GPIO ────────────────────────────────────────── */
#define BQ25895_REG0F_VBUS_MASK        0x80U  /* bit 7 */

/* ── REG10: Direction Control ──────────────────────────────────────────── */
#define BQ25895_REG10_EN_CHRG_PIN      BIT(4)

/* ── REG11: OTG Boost Control ──────────────────────────────────────────── */
#define BQ25895_REG11_EN_BOOST         BIT(7)
#define BQ25895_REG11_BOOST_LIM_MASK   0x70U  /* bits [6:4] */

/* ── REG12: OTG Voltage / Current Limit ────────────────────────────────── */
#define BQ25895_REG12_VOTG_MASK        0xF0U  /* bits [7:4] */
#define BQ25895_REG12_VOTG_SHIFT       4

/* ── REG14: Device / Revision ──────────────────────────────────────────── */
#define BQ25895_REG14_PN_MASK          0xF8U  /* bits [7:3] */
#define BQ25895_REG14_PN_SHIFT         3
#define BQ25895_REG14_REV_MASK         0x07U  /* bits [2:0] */

/* ── VBUS status values ────────────────────────────────────────────────── */
typedef enum {
    BQ25895_VBUS_NONE       = 0x00,
    BQ25895_VBUS_USB_SDP    = 0x01,
    BQ25895_VBUS_USB_CDP    = 0x02,
    BQ25895_VBUS_USB_DCP    = 0x03,
    BQ25895_VBUS_MAX        = 0x04,
    BQ25895_VBUS_UNKNOWN    = 0x05,
    BQ25895_VBUS_OTG        = 0x06,
} bq25895_vbus_stat_t;

/* ── Charge status values ──────────────────────────────────────────────── */
typedef enum {
    BQ25895_CHRG_NOT_CHARGING = 0x00,
    BQ25895_CHRG_PRE_CHARGE   = 0x01,
    BQ25895_CHRG_FAST_CHARGE  = 0x02,
    BQ25895_CHRG_TERM_DONE    = 0x03,
} bq25895_chrg_stat_t;

/* ── Public API ────────────────────────────────────────────────────────── */

/**
 * @brief Read a single register.
 * @param reg   Register address.
 * @param val   Pointer to store value.
 * @return HAL status.
 */
HAL_StatusTypeDef bq25895_read_reg(bq25895_reg_t reg, uint8_t *val);

/**
 * @brief Write a single register.
 * @param reg   Register address.
 * @param val   Value to write.
 * @return HAL status.
 */
HAL_StatusTypeDef bq25895_write_reg(bq25895_reg_t reg, uint8_t val);

/**
 * @brief Read-modify-write a register.
 * @param reg       Register address.
 * @param mask      Bits to modify.
 * @param val       New value (unmasked).
 * @return HAL status.
 */
HAL_StatusTypeDef bq25895_update_reg(bq25895_reg_t reg, uint8_t mask, uint8_t val);

/**
 * @brief Probe I2C and verify chip ID.
 * @return true if device responds and PN matches.
 */
bool bq25895_init(void);

/**
 * @brief Read current VBUS status.
 */
bq25895_vbus_stat_t bq25895_get_vbus_status(void);

/**
 * @brief Read charge status.
 */
bq25895_chrg_stat_t bq25895_get_charge_status(void);

/**
 * @brief Check power-good status.
 */
bool bq25895_is_power_good(void);

/**
 * @brief Set input current limit (mA).
 */
HAL_StatusTypeDef bq25895_set_input_current_limit(uint16_t ma);

/**
 * @brief Set fast-charge current limit (mA).
 */
HAL_StatusTypeDef bq25895_set_charge_current(uint16_t ma);

/**
 * @brief Set charge voltage limit (mV).
 */
HAL_StatusTypeDef bq25895_set_charge_voltage(uint16_t mv);

/**
 * @brief Enable or disable charging via CE pin + register.
 */
void bq25895_enable_charging(bool enable);

/**
 * @brief Read fault register and return decoded value.
 */
uint8_t bq25895_get_faults(void);

#endif /* BQ25895_H */
