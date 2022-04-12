#include "my_sys.h"


//��Ҫcubemx����ʱSYSʱ��Դѡ��SysTick,������Ч
void HAL_Delay_us(uint32_t us)//��ʱus����
{
    __IO uint32_t currentTicks = SysTick->VAL;
  /* Number of ticks per millisecond */
  const uint32_t tickPerMs = SysTick->LOAD + 1;
  /* Number of ticks to count */
  const uint32_t nbTicks = ((us - ((us > 0) ? 1 : 0)) * tickPerMs) / 1000;
  /* Number of elapsed ticks */
  uint32_t elapsedTicks = 0;
  __IO uint32_t oldTicks = currentTicks;
  do {
    currentTicks = SysTick->VAL;
    elapsedTicks += (oldTicks < currentTicks) ? tickPerMs + oldTicks - currentTicks :
                    oldTicks - currentTicks;
    oldTicks = currentTicks;
  } while (nbTicks > elapsedTicks);
}


#if SYS_Delay_Tim_En//ʹ��Tim����ʱms��us
	void HAL_TimDelay_us(uint32_t us)//��ʱus����
	{
		__HAL_TIM_SetCounter(&US_TIM,0);//����
		HAL_TIM_Base_Start(&US_TIM);//������ʱ��
		while(__HAL_TIM_GetCounter(&US_TIM)<us){};
		HAL_TIM_Base_Stop(&US_TIM);//�رն�ʱ��
	}
	void HAL_TimDelay_ms(uint32_t ms)//��ʱms����
	{
		for(uint32_t Delay_Cnt=0;Delay_Cnt<ms;Delay_Cnt++)
		HAL_TimDelay_us(998);//����ʵ�ʴ���������ģ���д1000����һ��㲹��
	}
#endif

//��ȡϵͳʱ�䣬��λms
uint32_t millis(void)
{
	return HAL_GetTick();
}


__STATIC_INLINE uint32_t GXT_SYSTICK_IsActiveCounterFlag(void)
{
  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}
static uint32_t getCurrentMicros(void)
{
  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
  GXT_SYSTICK_IsActiveCounterFlag();
  uint32_t m = HAL_GetTick();
  const uint32_t tms = SysTick->LOAD + 1;
  __IO uint32_t u = tms - SysTick->VAL;
  if (GXT_SYSTICK_IsActiveCounterFlag()) {
    m = HAL_GetTick();
    u = tms - SysTick->VAL;
  }
  return (m * 1000 + (u * 1000) / tms);
}
//��ȡϵͳʱ�䣬��λus
uint32_t micros(void)
{
  return getCurrentMicros();
}




