/*
 * Copyright (c) 2018, GreenWaves Technologies, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of GreenWaves Technologies, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "system_gap8.h"

/* PMSIS includes. */
#include "pmsis.h"
#include "pmsis_driver/pmu/pmsis_pmu.h"

#if defined(__SEMIHOSTING__)
#include "semihost.h"
#endif  /* __SEMIHOSTING__ */

/* FC & L2 heaps. */
extern char __heapfcram_start;
extern char __heapfcram_size;
extern char __heapl2ram_start;
extern char __heapl2ram_size;

volatile uint32_t SystemCoreClock = (uint32_t) DEFAULT_SYSTEM_CLOCK;

void system_init(void)
{
    /* Deactivate all soc events as they are active by default */
    SOCEU->FC_MASK_MSB = 0xFFFFFFFF;
    SOCEU->FC_MASK_LSB = 0xFFFFFFFF;

    /* FC Icache Enable*/
    SCBC->ICACHE_ENABLE = 0xFFFFFFFF;

    /* Setup FC_SOC events handler. */
    pi_fc_event_handler_init(FC_SOC_EVENT_IRQN);

    /* PMU Init */
    pi_pmu_init();

    /* Enable IRQ. */
    __enable_irq();

    /* Initialize malloc functions. */
    pmsis_malloc_init((void*) &__heapfcram_start, (uint32_t) &__heapfcram_size,
                      (void*) &__heapl2ram_start, (uint32_t) &__heapl2ram_size);

    #if defined(PRINTF_UART)
    printf_uart_init();
    #endif  /* PRINTF_UART */
}

void system_setup_systick(uint32_t tick_rate_hz)
{
    /* Systick timer configuration. */
    SysTick->CFG_REG_LO = (   ( 1 << SysTick_CFG_REG_LOW_ENABLE_Pos )
                            | ( 1 << SysTick_CFG_REG_LOW_RESET_Pos )
                            | ( 1 << SysTick_CFG_REG_LOW_IRQE_Pos )
                            | ( 0 << SysTick_CFG_REG_LOW_IEM_Pos )
                            | ( 1 << SysTick_CFG_REG_LOW_CMP_CLR_Pos )
                            | ( 0 << SysTick_CFG_REG_LOW_ONE_SHOT_Pos )
                            | ( 0 << SysTick_CFG_REG_LOW_PRESCALERE_Pos )
                            | ( 0 << SysTick_CFG_REG_LOW_CLKS_Pos )
                            | ( 0 << SysTick_CFG_REG_LOW_PRESCALER_Pos )
                            | ( 0 << SysTick_CFG_REG_LOW_64BIT_Pos )
        );
    /* Start the timer by putting a CMP value. */
    SysTick->CMP_LO = ( system_core_clock_get() / tick_rate_hz ) - 1ul;
    SysTick->VALUE_LO = 0;
    /* Enable IRQ from Systick timer. */
    NVIC->MASK_IRQ_OR = (0x1 << SYSTICK_IRQN);
}

void system_core_clock_update(void)
{
    SystemCoreClock = pi_fll_get_frequency(FLL_SOC);
}

uint32_t system_core_clock_get(void)
{
    system_core_clock_update();
    return SystemCoreClock;
}

void system_exit(int32_t code)
{
    if (pi_is_fc())
    {
        /* Write return value to APB device */
        soc_ctrl_corestatus_set(code);

        #if defined(__SEMIHOSTING__)
        semihost_exit(code == 0 ? SEMIHOST_EXIT_SUCCESS : SEMIHOST_EXIT_ERROR);
        #else
        /* Flush the pending messages to the debug tools
           Notify debug tools about the termination */
        BRIDGE_PrintfFlush();
        DEBUG_Exit(DEBUG_GetDebugStruct(), code);
        BRIDGE_SendNotif();
        #endif  /* __SEMIHOSTING__ */

    }
    /* In case the platform does not support exit or this core is not allowed to exit the platform ... */
    hal_eu_evt_mask_clr(0xffffffff);
    hal_eu_evt_wait();
}
