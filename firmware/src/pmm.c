/**
 * @file pmm.c
 * @brief PMM system management — peripheral init, state machine, LED.
 */

#include "pmm.h"
#include "pmm_config.h"
#include <stdio.h>
#include <string.h>

/* ── HAL handles (file scope) ──────────────────────────────────────────── */
I2C_HandleTypeDef  hi2c1;
UART_HandleTypeDef huart2;
ADC_HandleTypeDef  hadc1;

static pmm_status_t  status;
static uint32_t      last_poll_tick;
static uint32_t      last_led_tick;
static uint32_t      last_print_tick;
static bool          led_state;

/* ── Forward declarations ──────────────────────────────────────────────── */
static void SystemClock_Config(void);
static void GPIO_Init(void);
static void I2C1_Init(void);
static void USART2_Init(void);
static void ADC1_Init(void);
static void poll_peripherals(void);
static void led_update(uint32_t now);
static void update_state(void);

/* ── External I2C handle setters (defined in driver .c files) ──────────── */
extern void bq25895_set_i2c_handle(I2C_HandleTypeDef *handle);
extern void bq27441_set_i2c_handle(I2C_HandleTypeDef *handle);

/* ────────────────────────────────────────────────────────────────────────
 * Clock configuration (HSI 48 MHz, no HSE)
 * ──────────────────────────────────────────────────────────────────────── */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};

    /* HSI is 48 MHz; no PLL required on this device. */
    osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    osc.HSIState       = RCC_HSI_ON;
    osc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    HAL_RCC_OscConfig(&osc);

    /* SYSCLK = HSI, AHB prescaler = 1 */
    clk.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK;
    clk.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    clk.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_0);
}

/* ────────────────────────────────────────────────────────────────────────
 * GPIO initialisation
 * ──────────────────────────────────────────────────────────────────────── */
static void GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef g = {0};

    /* Outputs: PA0 (load SW), PA4 (charger CE), PA6 (LED),
                PA7 (PMM_INT), PA11 (SHDN), PA12 (FORCE_ON) */
    g.Mode  = GPIO_MODE_OUTPUT_PP;
    g.Pull  = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;

    g.Pin = LOAD_SW_PIN;
    HAL_GPIO_Init(LOAD_SW_PORT, &g);

    g.Pin = CHG_CE_PIN;
    HAL_GPIO_Init(CHG_CE_PORT, &g);

    g.Pin = LED_PIN;
    HAL_GPIO_Init(LED_PORT, &g);

    g.Pin = PMM_INT_PIN;
    HAL_GPIO_Init(PMM_INT_PORT, &g);

    g.Pin = SHDN_PIN;
    HAL_GPIO_Init(SHDN_PORT, &g);

    g.Pin = FORCE_ON_PIN;
    HAL_GPIO_Init(FORCE_ON_PORT, &g);

    /* Default states */
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);           /* LED off */
    HAL_GPIO_WritePin(PMM_INT_PORT, PMM_INT_PIN, GPIO_PIN_RESET);  /* INT low */
    HAL_GPIO_WritePin(SHDN_PORT, SHDN_PIN, GPIO_PIN_RESET);        /* normal */
    HAL_GPIO_WritePin(FORCE_ON_PORT, FORCE_ON_PIN, GPIO_PIN_SET);  /* force on */
    HAL_GPIO_WritePin(LOAD_SW_PORT, LOAD_SW_PIN, GPIO_PIN_SET);   /* load on */
    HAL_GPIO_WritePin(CHG_CE_PORT, CHG_CE_PIN, GPIO_PIN_SET);     /* charger disabled (CE active low) */

    /* Inputs with pull-up: PA1 (BQ27441 INT), PA5 (BQ25895 INT) */
    g.Mode  = GPIO_MODE_INPUT;
    g.Pull  = GPIO_PULLUP;

    g.Pin = BQ27441_INT_PIN;
    HAL_GPIO_Init(BQ27441_INT_PORT, &g);

    g.Pin = BQ25895_INT_PIN;
    HAL_GPIO_Init(BQ25895_INT_PORT, &g);

    /* ADC input: PA8 (analog) */
    g.Mode  = GPIO_MODE_ANALOG;
    g.Pull  = GPIO_NOPULL;
    g.Pin   = PMM_ADC_PIN;
    HAL_GPIO_Init(PMM_ADC_PORT, &g);
}

/* ────────────────────────────────────────────────────────────────────────
 * I2C1 initialisation (PA9=SCL, PA10=SDA, 400 kHz)
 * ──────────────────────────────────────────────────────────────────────── */
static void I2C1_Init(void)
{
    __HAL_RCC_I2C1_CLK_ENABLE();

    /* Configure PA9 (SCL) and PA10 (SDA) as alternate-open-drain */
    GPIO_InitTypeDef g = {0};
    g.Mode      = GPIO_MODE_AF_OD;
    g.Pull      = GPIO_PULLUP;
    g.Speed     = GPIO_SPEED_FREQ_HIGH;
    g.Alternate = GPIO_AF6_I2C1;

    g.Pin = PMM_I2C_SCL_PIN;
    HAL_GPIO_Init(PMM_I2C_SCL_PORT, &g);

    g.Pin = PMM_I2C_SDA_PIN;
    HAL_GPIO_Init(PMM_I2C_SDA_PORT, &g);

    /* I2C peripheral config */
    hi2c1.Instance             = PMM_I2C;
    hi2c1.Init.Timing          = 0x10B17DB5;  /* 400 kHz @ 48 MHz HSI */
    hi2c1.Init.OwnAddress1     = 0;
    hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c1);

    /* Pass handle to drivers */
    bq25895_set_i2c_handle(&hi2c1);
    bq27441_set_i2c_handle(&hi2c1);
}

/* ────────────────────────────────────────────────────────────────────────
 * USART2 initialisation (PA2=TX, PA3=RX, 115200 baud)
 * ──────────────────────────────────────────────────────────────────────── */
static void USART2_Init(void)
{
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitTypeDef g = {0};
    g.Mode      = GPIO_MODE_AF_PP;
    g.Pull      = GPIO_NOPULL;
    g.Speed     = GPIO_SPEED_FREQ_HIGH;
    g.Alternate = GPIO_AF0_USART2;

    g.Pin = PMM_UART_TX_PIN;
    HAL_GPIO_Init(PMM_UART_TX_PORT, &g);

    g.Pull  = GPIO_PULLUP;
    g.Pin   = PMM_UART_RX_PIN;
    HAL_GPIO_Init(PMM_UART_RX_PORT, &g);

    huart2.Instance          = PMM_UART;
    huart2.Init.BaudRate     = PMM_UART_BAUD;
    huart2.Init.WordLength   = UART_WORDLENGTH_8B;
    huart2.Init.StopBits     = UART_STOPBITS_1;
    huart2.Init.Parity       = UART_PARITY_NONE;
    huart2.Init.Mode         = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
}

/* ────────────────────────────────────────────────────────────────────────
 * ADC1 initialisation (single-shot, PA8)
 * ──────────────────────────────────────────────────────────────────────── */
static void ADC1_Init(void)
{
    __HAL_RCC_ADC_CLK_ENABLE();

    ADC_ChannelConfTypeDef ch = {0};

    hadc1.Instance                   = PMM_ADC;
    hadc1.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution            = ADC_RESOLUTION_12B;
    hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode          = DISABLE;
    hadc1.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
    hadc1.Init.LowPowerAutoWait      = DISABLE;
    hadc1.Init.ContinuousConvMode    = DISABLE;
    hadc1.Init.NbrOfConversion       = 1;
    hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    HAL_ADC_Init(&hadc1);

    ch.Channel      = PMM_ADC_CH;
    ch.Rank         = ADC_REGULAR_RANK_1;
    ch.SamplingTime = ADC_SAMPLETIME_39CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &ch);
}

/* ── ADC single-shot read ──────────────────────────────────────────────── */
static uint32_t adc_read_mv(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 50);
    uint32_t raw = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    /* Assume a resistor divider that gives 1:2 (VDD reference = 3.3V) */
    return (raw * 3300U * 2U) / 4095U;
}

/* ── State machine ─────────────────────────────────────────────────────── */
static void update_state(void)
{
    status.vbus_stat      = bq25895_get_vbus_status();
    status.chrg_stat      = bq25895_get_charge_status();
    status.power_good     = bq25895_is_power_good();
    status.charger_faults = bq25895_get_faults();
    status.vbus_mv        = adc_read_mv();
    status.load_switch_on = (HAL_GPIO_ReadPin(LOAD_SW_PORT, LOAD_SW_PIN) == GPIO_PIN_SET);
    status.led_on         = led_state;

    /* Map hardware status to PMM state */
    if (status.charger_faults & 0x80) {
        status.state = PMM_STATE_FAULT;
    } else if (status.chrg_stat == BQ25895_CHRG_TERM_DONE) {
        status.state = PMM_STATE_FULL;
    } else if (status.chrg_stat == BQ25895_CHRG_FAST_CHARGE ||
               status.chrg_stat == BQ25895_CHRG_PRE_CHARGE) {
        status.state = PMM_STATE_CHARGING;
    } else if (status.vbus_stat == BQ25895_VBUS_NONE && status.batt.voltage_mv > 3000) {
        status.state = PMM_STATE_DISCHARGING;
    } else {
        status.state = PMM_STATE_IDLE;
    }
}

/* ── Peripheral polling (called once per main-loop iteration) ──────────── */
static void poll_peripherals(void)
{
    bq27441_read_all(&status.batt);
    update_state();
}

/* ── LED blinker ───────────────────────────────────────────────────────── */
static void led_update(uint32_t now)
{
    if (now - last_led_tick >= LED_BLINK_PERIOD_MS) {
        last_led_tick = now;
        led_state = !led_state;
        HAL_GPIO_WritePin(LED_PORT, LED_PIN,
                          led_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

/* ── printf retarget to USART2 ─────────────────────────────────────────── */
#ifdef __GNUC__
int _write(int fd, char *ptr, int len)
{
    (void)fd;
    HAL_UART_Transmit(&huart2, (uint8_t *)ptr, (uint16_t)len, 100);
    return len;
}
#else
int fputc(int ch, FILE *f)
{
    (void)f;
    uint8_t c = (uint8_t)ch;
    HAL_UART_Transmit(&huart2, &c, 1, 100);
    return ch;
}
#endif

/* ── Public API ────────────────────────────────────────────────────────── */

void pmm_init(void)
{
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    I2C1_Init();
    USART2_Init();
    ADC1_Init();

    memset(&status, 0, sizeof(status));
    last_poll_tick = 0;
    last_led_tick  = 0;
    last_print_tick = 0;
    led_state      = false;

    /* Probe I2C devices */
    bool charger_ok = bq25895_init();
    bool gauge_ok   = bq27441_init();

    printf("\r\n=== PMM-1.0-R1 ===\r\n");
    printf("MCU : STM32C031C4 @ %lu MHz\r\n", HAL_RCC_GetSysClockFreq() / 1000000U);
    printf("I2C : 400 kHz (PA9 SCL / PA10 SDA)\r\n");
    printf("UART: 115200 (PA2 TX / PA3 RX)\r\n");
    printf("BQ25895: %s (addr 0x%02X)\r\n", charger_ok ? "OK" : "NOT FOUND", BQ25895_I2C_ADDR);
    printf("BQ27441: %s (addr 0x%02X)\r\n", gauge_ok   ? "OK" : "NOT FOUND", BQ27441_I2C_ADDR);

    if (charger_ok) {
        /* Sensible defaults: 2A input, 1A charge, 4.2V target */
        bq25895_set_input_current_limit(2000);
        bq25895_set_charge_current(1024);
        bq25895_set_charge_voltage(4200);
        bq25895_enable_charging(true);
    }

    printf("PMM init complete.\r\n");
}

void pmm_poll(void)
{
    uint32_t now = HAL_GetTick();

    if (now - last_poll_tick >= POLL_INTERVAL_MS) {
        last_poll_tick = now;
        poll_peripherals();
    }

    if (now - last_print_tick >= STATUS_PRINT_MS) {
        last_print_tick = now;
        pmm_print_status();
    }

    led_update(now);
}

const pmm_status_t *pmm_get_status(void)
{
    return &status;
}

void pmm_set_load_switch(bool on)
{
    status.load_switch_on = on;
    HAL_GPIO_WritePin(LOAD_SW_PORT, LOAD_SW_PIN,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void pmm_set_charging(bool enable)
{
    bq25895_enable_charging(enable);
}

void pmm_shutdown(void)
{
    printf("PMM shutdown requested.\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t *)"", 0, 10);
    HAL_GPIO_WritePin(SHDN_PORT, SHDN_PIN, GPIO_PIN_SET);
    __WFI();
}

void pmm_print_status(void)
{
    float temp_c = (status.batt.temperature_01k / 10.0f) - 273.15f;

    printf("\r\n--- PMM Status ---\r\n");
    printf("State  : %d\r\n", status.state);
    printf("VBUS   : %lu mV  (PG=%d)\r\n", (unsigned long)status.vbus_mv, status.power_good);
    printf("VBUS src: %d\r\n", status.vbus_stat);
    printf("CHRG   : %d\r\n", status.chrg_stat);
    printf("Batt V : %u mV\r\n", status.batt.voltage_mv);
    printf("Batt I : %d mA\r\n", status.batt.current_ma);
    printf("Temp   : %.1f C\r\n", temp_c);
    printf("SoC    : %u %%\r\n", status.batt.soc_percent);
    printf("RemCap : %u mAh\r\n", status.batt.remaining_cap_mah);
    printf("FullCap: %u mAh\r\n", status.batt.full_cap_mah);
    printf("Design : %u mAh\r\n", status.batt.design_cap_mah);
    printf("TTE    : %u min\r\n", status.batt.tte_min);
    printf("TTF    : %u min\r\n", status.batt.ttf_min);
    printf("AvgPwr : %d mW\r\n", status.batt.avg_power_mw);
    printf("SoH    : %u %%\r\n", status.batt.soh_percent);
    printf("Flags  : 0x%04X\r\n", status.batt.flags);
    printf("Faults : 0x%02X\r\n", status.charger_faults);
    printf("LoadSw : %s\r\n", status.load_switch_on ? "ON" : "OFF");
}
