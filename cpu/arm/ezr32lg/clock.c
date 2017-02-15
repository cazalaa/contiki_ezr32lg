#include <stdio.h>
#include "contiki.h"
#include "ezr32xx_conf.h"
#include "ezr32xx_systick.h"

/* After how many clock cycles should the systick interrupt be fired */
#define RELOAD_VALUE ((F_CPU/CLOCK_CONF_SECOND) - 1)

static volatile unsigned long seconds;
static volatile clock_time_t ticks;

/* This interrupt function will increase the tick counter */
void SysTick_Handler(void)
{
  ENERGEST_ON(ENERGEST_TYPE_IRQ);
  ticks++;

  if((ticks % CLOCK_SECOND) == 0) {
    seconds++;
    energest_flush();
  }

  /* If an etimer expired, continue its process */
  if(etimer_pending()) {
    etimer_request_poll();
  }

  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

void clock_init(void) {
  ticks = 0;
  seconds = 0;

  /* Select the main processor as systick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  /* Set the reload value */
  SysTick_SetReload(RELOAD_VALUE);

  /* Enable the systick interrupt */
  SysTick_ITConfig(ENABLE);

  /* Enable the systick timer */
  SysTick_CounterCmd(SysTick_Counter_Enable);
}

unsigned long clock_seconds(void) {
  return seconds;
}

void clock_set_seconds(unsigned long sec) {
  seconds = sec;
}

clock_time_t clock_time(void) {
  return ticks;
}

/* Busy-wait the CPU for a duration depending on CPU speed */
void clock_delay(unsigned int i) {
  for(; i > 0; i--) {
    unsigned int j;
    for(j = 50; j > 0; j--) {
      __NOP();
    }
  }
}

/* Wait for a multiple of clock ticks (3.9ms per tick at 256Hz) */
void clock_wait(clock_time_t i) {
  clock_time_t start;
  start = clock_time();
  while(clock_time() - start < i);
}
