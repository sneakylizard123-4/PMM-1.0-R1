/**
 * @file bq27441.c
 * @brief BQ27441-G1 battery fuel gauge I2C driver implementation.
 *
 * I2C protocol: write 2-byte command code, then read 2 bytes of data.
 */

#include "bq27441.h"
#include "pmm_config.h"
#include <string.h>

/* ── Private state ─────────────────────────────────────────────────────── */
static I2C_HandleTypeDef *hi2c;

/* ── Low-level I2C helpers ─────────────────────────────────────────────── */

void bq27441_set_i2c_handle(I2C_HandleTypeDef *handle)
{
    hi2c = handle;
}

/**
 * @brief Write a 16-bit command code, then read 16 bits of data.
 */
static HAL_StatusTypeDef bq27441_xfer(uint16_t cmd, uint16_t *data)
{
    uint8_t buf[2];
    HAL_StatusTypeDef ret;

    /* Write command code (MSB first) */
    buf[0] = (uint8_t)(cmd >> 8);
    buf[1] = (uint8_t)(cmd & 0xFF);
    ret = HAL_I2C_Master_Transmit(hi2c, BQ27441_ADDR_W, buf, 2, 100);
    if (ret != HAL_OK)
        return ret;

    /* Read 2 bytes of data (MSB first) */
    ret = HAL_I2C_Master_Receive(hi2c, BQ27441_ADDR_R, buf, 2, 100);
    if (ret != HAL_OK)
        return ret;

    *data = ((uint16_t)buf[0] << 8) | buf[1];
    return HAL_OK;
}

/* ── Public: command-level access ──────────────────────────────────────── */

uint16_t bq27441_read_cmd(bq27441_cmd_t cmd)
{
    uint16_t val = 0xFFFF;
    bq27441_xfer((uint16_t)cmd, &val);
    return val;
}

HAL_StatusTypeDef bq27441_control_cmd(bq27441_ctrl_cmd_t ctrl)
{
    uint16_t dummy;
    return bq27441_xfer((uint16_t)ctrl, &dummy);
}

/* ── Init / probe ──────────────────────────────────────────────────────── */

bool bq27441_init(void)
{
    uint16_t devtype = bq27441_read_cmd(BQ27441_CMD_DEVICE_TYPE);
    return (devtype == 0x0441);
}

/* ── Individual reads ──────────────────────────────────────────────────── */

uint16_t bq27441_read_voltage(void)
{
    return bq27441_read_cmd(BQ27441_CMD_VOLTAGE);
}

int16_t bq27441_read_current(void)
{
    return (int16_t)bq27441_read_cmd(BQ27441_CMD_CURRENT);
}

int16_t bq27441_read_temperature(void)
{
    return (int16_t)bq27441_read_cmd(BQ27441_CMD_TEMPERATURE);
}

uint16_t bq27441_read_soc(void)
{
    return bq27441_read_cmd(BQ27441_CMD_STATE_OF_CHARGE);
}

uint16_t bq27441_read_flags(void)
{
    return bq27441_read_cmd(BQ27441_CMD_FLAGS);
}

/* ── Aggregate read ────────────────────────────────────────────────────── */

void bq27441_read_all(bq27441_data_t *data)
{
    memset(data, 0, sizeof(*data));

    data->temperature_01k = (int16_t)bq27441_read_cmd(BQ27441_CMD_TEMPERATURE);
    data->voltage_mv      = bq27441_read_cmd(BQ27441_CMD_VOLTAGE);
    data->current_ma      = (int16_t)bq27441_read_cmd(BQ27441_CMD_CURRENT);
    data->avg_current_ma  = (int16_t)bq27441_read_cmd(BQ27441_CMD_AVG_CURRENT);
    data->soc_percent     = bq27441_read_cmd(BQ27441_CMD_STATE_OF_CHARGE);
    data->remaining_cap_mah = bq27441_read_cmd(BQ27441_CMD_REMAINING_CAP);
    data->full_cap_mah    = bq27441_read_cmd(BQ27441_CMD_FULL_CHARGE_CAP);
    data->design_cap_mah  = bq27441_read_cmd(BQ27441_CMD_DESIGN_CAPACITY);
    data->flags           = bq27441_read_cmd(BQ27441_CMD_FLAGS);
    data->tte_min         = bq27441_read_cmd(BQ27441_CMD_TIME_TO_EMPTY);
    data->ttf_min         = bq27441_read_cmd(BQ27441_CMD_TIME_TO_FULL);
    data->avg_power_mw    = (int16_t)bq27441_read_cmd(BQ27441_CMD_AVG_POWER);
    data->soh_percent     = (uint8_t)bq27441_read_cmd(BQ27441_CMD_STATE_OF_HEALTH);
}

/* ── Hibernate ─────────────────────────────────────────────────────────── */

HAL_StatusTypeDef bq27441_enter_hibernate(void)
{
    return bq27441_control_cmd(BQ27441_CTRL_SET_HIBERNATE);
}

HAL_StatusTypeDef bq27441_exit_hibernate(void)
{
    return bq27441_control_cmd(BQ27441_CTRL_CLEAR_HIBERNATE);
}
