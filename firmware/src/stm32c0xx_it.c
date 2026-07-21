/**
 * @file stm32c0xx_it.c
 * @brief Cortex-M0+ interrupt handlers for PMM firmware.
 */

#include "stm32c0xx_hal.h"

/* ── External HAL handles (defined in pmm.c) ───────────────────────────── */
extern I2C_HandleTypeDef  hi2c1;
extern UART_HandleTypeDef huart2;
extern ADC_HandleTypeDef  hadc1;

/* ── Cortex-M0+ system exceptions ──────────────────────────────────────── */

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    for (;;) {
        __NOP();
    }
}

void SVC_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

/* ── Peripheral interrupt handlers ─────────────────────────────────────── */

void I2C1_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&hi2c1);
    HAL_I2C_ER_IRQHandler(&hi2c1);
}

void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2);
}

void ADC1_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&hadc1);
}
