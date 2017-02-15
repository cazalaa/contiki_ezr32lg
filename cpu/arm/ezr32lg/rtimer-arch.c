#include "rtimer-arch.h"

/* The prescaler assumes the timer is sourced with same clock speed as the CPU */
#define RTIMER_PRESCALER (F_CPU / (RTIMER_SECOND*2))

static volatile rtimer_clock_t rtimer_clock;
static volatile rtimer_clock_t timeout_value;

void TIM2_IRQHandler(void) {
	ENERGEST_ON(ENERGEST_TYPE_IRQ);
	rtimer_clock++;

	/* Clear interrupt pending flag */
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	/* Check for, and run, any expired rtimers */
	if(rtimer_clock==timeout_value){
		rtimer_run_next();
	}

	ENERGEST_OFF(ENERGEST_TYPE_IRQ);
}

void rtimer_arch_init(void) {
	rtimer_clock = 0;
	timeout_value = 0;

	/* To init, we use structures that are filled in and passed to the HAL */
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* Enable TIM2 peripheral clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Enable the TIM2 Interrupt in the NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Configure TIM2 */
	TIM_TimeBaseStructure.TIM_Period = 1;
	TIM_TimeBaseStructure.TIM_Prescaler = RTIMER_PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Enable TIM2 & TIM2 Update interrupt */
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

rtimer_clock_t rtimer_arch_now(void) {
	return rtimer_clock;
}


void rtimer_arch_schedule(rtimer_clock_t t) {
	timeout_value = rtimer_clock + t;
}

