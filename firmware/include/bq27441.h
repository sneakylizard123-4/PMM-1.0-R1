/**
 * @file bq27441.h
 * @brief BQ27441-G1 battery fuel gauge I2C driver (addr 0x55).
 *
 * Uses standard 16-bit command protocol per TI SLUUb02 datasheet.
 */

#ifndef BQ27441_H
#define BQ27441_H

#include "stm32c0xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#ifndef BIT
#define BIT(x) (1UL << (x))
#endif

/* ── I2C address ───────────────────────────────────────────────────────── */
#define BQ27441_ADDR_W  ((BQ27441_I2C_ADDR << 1) | 0)   /* 0xAA */
#define BQ27441_ADDR_R  ((BQ27441_I2C_ADDR << 1) | 1)   /* 0xAB */

/* ── Standard Command codes (16-bit) ───────────────────────────────────── */
typedef enum {
    BQ27441_CMD_CONTROL              = 0x0000,
    BQ27441_CMD_CONTROL_STATUS       = 0x0002,
    BQ27441_CMD_DEVICE_TYPE          = 0x0004,
    BQ27441_CMD_FW_VERSION           = 0x0006,
    BQ27441_CMD_HW_VERSION           = 0x0008,
    BQ27441_CMD_MANUFACTURER_DATE    = 0x000A,
    BQ27441_CMD_SERIAL_NUMBER        = 0x000C,
    BQ27441_CMD_BATTERY_STATUS       = 0x0010,
    BQ27441_CMD_TEMPERATURE          = 0x0012,
    BQ27441_CMD_VOLTAGE              = 0x0014,
    BQ27441_CMD_FLAGS                = 0x0016,
    BQ27441_CMD_NOMINAL_AVAIL_CAP   = 0x0018,
    BQ27441_CMD_FULL_AVAIL_CAP      = 0x001A,
    BQ27441_CMD_REMAINING_CAP       = 0x001C,
    BQ27441_CMD_FULL_CHARGE_CAP     = 0x001E,
    BQ27441_CMD_AVG_CURRENT         = 0x0020,
    BQ27441_CMD_AVG_TIME_TO_EMPTY   = 0x0022,
    BQ27441_CMD_AVG_TIME_TO_FULL    = 0x0024,
    BQ27441_CMD_CURRENT             = 0x0026,
    BQ27441_CMD_TIME_TO_EMPTY       = 0x0028,
    BQ27441_CMD_TIME_TO_FULL        = 0x002A,
    BQ27441_CMD_STANDBY_CURRENT      = 0x002C,
    BQ27441_CMD_STANDBY_TIME_EMPTY  = 0x002E,
    BQ27441_CMD_STANDBY_TIME_FULL   = 0x0030,
    BQ27441_CMD_MAX_LOAD_CURRENT     = 0x0032,
    BQ27441_CMD_MAX_LOAD_TIME_EMPTY = 0x0034,
    BQ27441_CMD_RAW_COULOMB_COUNT   = 0x0036,
    BQ27441_CMD_AVG_POWER           = 0x0038,
    BQ27441_CMD_TTE                 = 0x003A,
    BQ27441_CMD_TTF                 = 0x003C,
    BQ27441_CMD_STATE_OF_HEALTH     = 0x003E,
    BQ27441_CMD_STATE_OF_CHARGE     = 0x0040,
    BQ27441_CMD_DESIGN_CAPACITY     = 0x0042,
    BQ27441_CMD_DESIGN_ENERGY       = 0x0044,
} bq27441_cmd_t;

/* ── Control sub-commands (write to CMD_CONTROL, read result) ──────────── */
typedef enum {
    BQ27441_CTRL_STATUS             = 0x0000,
    BQ27441_CTRL_DEVICE_TYPE        = 0x0001,
    BQ27441_CTRL_FW_VERSION         = 0x0002,
    BQ27441_CTRL_HW_VERSION         = 0x0003,
    BQ27441_CTRL_CHARGE_DESIGN      = 0x0004,
    BQ27441_CTRL_CHARGE_DESIGN_ENRG = 0x0005,
    BQ27441_CTRL_MAC_DATA           = 0x0006,
    BQ27441_CTRL_MAC_DATA_SUM       = 0x0007,
    BQ27441_CTRL_MAC_DATA_CHECK     = 0x0008,
    BQ27441_CTRL_MAC_CMD            = 0x0009,
    BQ27441_CTRL_BAT_INSERT         = 0x000C,
    BQ27441_CTRL_BAT_REMOVE         = 0x000D,
    BQ27441_CTRL_SET_HIBERNATE      = 0x0011,
    BQ27441_CTRL_CLEAR_HIBERNATE    = 0x0012,
    BQ27441_CTRL_SET_SLP_ENABLE     = 0x0013,
    BQ27441_CTRL_CLR_SLP_ENABLE     = 0x0014,
    BQ27441_CTRL_DEVICE_RESET       = 0x001E,
    BQ27441_CTRL_RESET_DATA         = 0x001F,
    BQ27441_CTRL_SOFT_RESET         = 0x0042,
    BQ27441_CTRL_SET_ACTIVE          = 0x001D,
    BQ27441_CTRL_CLEAR_ACTIVE        = 0x001C,
} bq27441_ctrl_cmd_t;

/* ── Bit masks for BQ27441_FLAGS register ──────────────────────────────── */
#define BQ27441_FLAG_DSG             BIT(0)
#define BQ27441_FLAG_SYSDWNL         BIT(1)
#define BQ27441_FLAG_SOC1            BIT(2)
#define BQ27441_FLAG_SOCF            BIT(3)
#define BQ27441_FLAG_SOCF_LOCK       BIT(4)
#define BQ27441_FLAG_SOC1_LOCK       BIT(5)
#define BQ27441_FLAG_DELTAF          BIT(6)
#define BQ27441_FLAG_INIT            BIT(7)

/* ── Battery status bits ───────────────────────────────────────────────── */
#define BQ27441_BATT_STATUS_DSG      BIT(0)
#define BQ27441_BATT_STATUS_SYSDWNL  BIT(1)
#define BQ27441_BATT_STATUS_SOC1     BIT(2)
#define BQ27441_BATT_STATUS_SOCF     BIT(3)
#define BQ27441_BATT_STATUS_FC       BIT(4)
#define BQ27441_BATT_STATUS_CHG_INH  BIT(5)
#define BQ27441_BATT_STATUS_UTC      BIT(7)

/* ── Data type for a complete fuel gauge snapshot ──────────────────────── */
typedef struct {
    int16_t  temperature_01k;   /* Temperature in 0.1 K */
    uint16_t voltage_mv;        /* Voltage in mV */
    int16_t  current_ma;        /* Signed: charge (+) / discharge (-) mA */
    int16_t  avg_current_ma;    /* Average current in mA */
    uint16_t soc_percent;       /* State of charge 0-100 % */
    uint16_t remaining_cap_mah; /* Remaining capacity in mAh */
    uint16_t full_cap_mah;      /* Full charge capacity in mAh */
    uint16_t design_cap_mah;    /* Design capacity in mAh */
    uint16_t flags;             /* Raw flags register */
    uint16_t tte_min;           /* Time to empty in minutes */
    uint16_t ttf_min;           /* Time to full in minutes */
    int16_t  avg_power_mw;      /* Average power in mW */
    uint8_t  soh_percent;       /* State of health 0-100 % */
} bq27441_data_t;

/* ── Public API ────────────────────────────────────────────────────────── */

/**
 * @brief Read a 16-bit standard command.
 * @param cmd   Command code.
 * @return 16-bit value.
 */
uint16_t bq27441_read_cmd(bq27441_cmd_t cmd);

/**
 * @brief Write a 16-bit control sub-command.
 * @param ctrl  Control sub-command code.
 * @return HAL status.
 */
HAL_StatusTypeDef bq27441_control_cmd(bq27441_ctrl_cmd_t ctrl);

/**
 * @brief Probe and identify the BQ27441.
 * @return true if device type is 0x0441.
 */
bool bq27441_init(void);

/**
 * @brief Read a complete snapshot of battery data.
 * @param data  Pointer to data structure to populate.
 */
void bq27441_read_all(bq27441_data_t *data);

/* ── Individual quick-reads ────────────────────────────────────────────── */
uint16_t bq27441_read_voltage(void);
int16_t  bq27441_read_current(void);
int16_t  bq27441_read_temperature(void);
uint16_t bq27441_read_soc(void);
uint16_t bq27441_read_flags(void);

/**
 * @brief Enable Hibernate mode to save power.
 */
HAL_StatusTypeDef bq27441_enter_hibernate(void);

/**
 * @brief Exit Hibernate mode.
 */
HAL_StatusTypeDef bq27441_exit_hibernate(void);

#endif /* BQ27441_H */
