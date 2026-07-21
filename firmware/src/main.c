/**
 * @file main.c
 * @brief PMM-1.0-R1 application entry point.
 *
 * Initialises the PMM system, then runs the main poll loop forever.
 * LED blinks at 2 Hz; UART prints status every second.
 */

#include "pmm.h"
#include "pmm_config.h"

int main(void)
{
    pmm_init();

    /* Main loop */
    for (;;) {
        pmm_poll();
    }
}
