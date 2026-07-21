/**
 * @file bq25895.c
 * @brief BQ25895 USB buck-charger I2C driver implementation.
 */

#include "bq25895.h"
#include "pmm_config.h"

/* ── Private state ─────────────────────────────────────────────────────── */
static I2C_HandleTypeDef *hi2c;

/* ── Low-level I2C helpers ─────────────────────────────────────────────── */

HAL_StatusTypeDef bq25895_read_reg(bq25895_reg_t reg, uint8_t *val)
{
    return HAL_I2C_Mem_Read(hi2c, BQ25895_ADDR_W, (uint16_t)reg,
                            I2C_MEMADD_SIZE_8BIT, val, 1, 100);
}

HAL_StatusTypeDef bq25895_write_reg(bq25895_reg_t reg, uint8_t val)
{
    return HAL_I2C_Mem_Write(hi2c, BQ25895_ADDR_W, (uint16_t)reg,
                             I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
}

HAL_StatusTypeDef bq25895_update_reg(bq25895_reg_t reg, uint8_t mask, uint8_t val)
{
    uint8_t tmp;
    HAL_StatusTypeDef ret = bq25895_read_reg(reg, &tmp);
    if (ret != HAL_OK) return ret;
    tmp = (tmp & ~mask) | (val & mask);
    return bq25895_write_reg(reg, tmp);
}

/* ── Initialisation ────────────────────────────────────────────────────── */

void bq25895_set_i2c_handle(I2C_HandleTypeDef *handle)
{
    hi2c = handle;
}

bool bq25895_init(void)
{
    /* Probe the device by reading the Device/Revision register. */
    uint8_t raw = 0;
    if (bq25895_read_reg(BQ25895_REG_14, &raw) != HAL_OK)
        return false;

    uint8_t pn = (raw & BQ25895_REG14_PN_MASK) >> BQ25895_REG14_PN_SHIFT;
    /* BQ25895 should report PN = 0x09 (decimal 9) */
    return (pn == 0x09);
}

/* ── Status queries ────────────────────────────────────────────────────── */

bq25895_vbus_stat_t bq25895_get_vbus_status(void)
{
    uint8_t reg;
    if (bq25895_read_reg(BQ25895_REG_08, &reg) != HAL_OK)
        return BQ25895_VBUS_UNKNOWN;

    if (!(reg & BQ25895_REG08_VBUS_GD))
        return BQ25895_VBUS_NONE;

    return (bq25895_vbus_stat_t)((reg & BQ25895_REG08_VBUS_STAT_MASK)
                                 >> BQ25895_REG08_VBUS_STAT_SHIFT);
}

bq25895_chrg_stat_t bq25895_get_charge_status(void)
{
    uint8_t reg;
    if (bq25895_read_reg(BQ25895_REG_08, &reg) != HAL_OK)
        return BQ25895_CHRG_NOT_CHARGING;

    return (bq25895_chrg_stat_t)((reg & BQ25895_REG08_CHRG_STAT_MASK)
                                 >> BQ25895_REG08_CHRG_STAT_SHIFT);
}

bool bq25895_is_power_good(void)
{
    uint8_t reg;
    if (bq25895_read_reg(BQ25895_REG_08, &reg) != HAL_OK)
        return false;
    return (reg & BQ25895_REG08_PG_STAT) != 0;
}

uint8_t bq25895_get_faults(void)
{
    uint8_t reg;
    bq25895_read_reg(BQ25895_REG_09, &reg);
    return reg;
}

/* ── Configuration ─────────────────────────────────────────────────────── */

HAL_StatusTypeDef bq25895_set_input_current_limit(uint16_t ma)
{
    if (ma < BQ25895_IINLIM_BASE_MA)
        ma = BQ25895_IINLIM_BASE_MA;
    uint8_t val = (uint8_t)((ma - BQ25895_IINLIM_BASE_MA) / BQ25895_IINLIM_STEP_MA);
    if (val > 0x3F) val = 0x3F;
    return bq25895_update_reg(BQ25895_REG_00,
                              BQ25895_REG00_IINLIM_MASK, val);
}

HAL_StatusTypeDef bq25895_set_charge_current(uint16_t ma)
{
    /* Each LSB = 64 mA, offset = 0 mA */
    uint8_t val = (uint8_t)(ma / BQ25895_REG04_ICHG_STEP_MA);
    if (val > 0x7F) val = 0x7F;
    return bq25895_update_reg(BQ25895_REG_04,
                              BQ25895_REG04_ICHG_MASK, val);
}

HAL_StatusTypeDef bq25895_set_charge_voltage(uint16_t mv)
{
    /* Each LSB = 16 mV, offset = 3840 mV */
    if (mv < BQ25895_REG06_VREG_BASE_MV)
        mv = BQ25895_REG06_VREG_BASE_MV;
    uint8_t val = (uint8_t)((mv - BQ25895_REG06_VREG_BASE_MV)
                            / BQ25895_REG06_VREG_STEP_MV);
    if (val > 0x7F) val = 0x7F;
    return bq25895_update_reg(BQ25895_REG_06,
                              BQ25895_REG06_VREG_MASK,
                              (uint8_t)(val << BQ25895_REG06_VREG_SHIFT));
}

void bq25895_enable_charging(bool enable)
{
    if (enable) {
        HAL_GPIO_WritePin(CHG_CE_PORT, CHG_CE_PIN, GPIO_PIN_RESET);  /* active low */
        bq25895_update_reg(BQ25895_REG_03, BQ25895_REG03_BAT_LOAD_EN,
                           BQ25895_REG03_BAT_LOAD_EN);
    } else {
        HAL_GPIO_WritePin(CHG_CE_PORT, CHG_CE_PIN, GPIO_PIN_SET);
        bq25895_update_reg(BQ25895_REG_03, BQ25895_REG03_BAT_LOAD_EN, 0);
    }
}
