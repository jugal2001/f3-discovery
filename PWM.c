#include "stm32f3xx.h"

volatile int ticks;

void SysTick_Handler(void)
{
	ticks++;
}

void delay_ms(int ms)
{
	ticks=0;
	while(ticks<ms);
}
void pwm_init()
{
	RCC ->APB1ENR |= RCC_APB1ENR_TIM2EN;	//enable clock for TIM2
	TIM2 ->CCER |= TIM_CCER_CC1E; 		//enable channel 1
	TIM2 ->CR1 |= TIM_CR1_ARPE;
  TIM2 ->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE; //pwm mode 1
	TIM2 ->PSC = 1;		//prescaler
	TIM2 ->ARR = 1023;
	TIM2 ->CCR1 = 1023;
	TIM2 ->EGR = TIM_EGR_UG; //update registers
	TIM2 ->CR1 = TIM_CR1_CEN; // start timer
}


void pwm_duty(int duty)
{
	TIM2->CCR1 = (duty*(TIM2->ARR)/100);
}
int main()
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= GPIO_MODER_MODER15_1;   //PA15 alternate function mode
	GPIOA->AFR[1] |= 1<<28;  		//AF1 (TIM2 CH1 output)
	
	
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
	
	pwm_init();
while(1)
{
	for(int i = 0; i<=100 ; i++)
	{
		pwm_duty(i);
		delay_ms(10);
	}
	for(int i = 100; i>=0 ; i--)
	{
		pwm_duty(i);
		delay_ms(10);
	}
}
}

