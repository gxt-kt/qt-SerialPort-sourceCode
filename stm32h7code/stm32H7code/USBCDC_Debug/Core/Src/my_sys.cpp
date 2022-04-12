#include "my_sys.h"


//需要cubemx配置时SYS时基源选择SysTick,否则无效
void HAL_Delay_us(uint32_t us)//延时us函数
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


#if SYS_Delay_Tim_En//使用Tim来延时ms或us
	void HAL_TimDelay_us(uint32_t us)//延时us函数
	{
		__HAL_TIM_SetCounter(&US_TIM,0);//清零
		HAL_TIM_Base_Start(&US_TIM);//开启定时器
		while(__HAL_TIM_GetCounter(&US_TIM)<us){};
		HAL_TIM_Base_Stop(&US_TIM);//关闭定时器
	}
	void HAL_TimDelay_ms(uint32_t ms)//延时ms函数
	{
		for(uint32_t Delay_Cnt=0;Delay_Cnt<ms;Delay_Cnt++)
		HAL_TimDelay_us(998);//考虑实际代码运行损耗，不写1000，差一点点补上
	}
#endif

//获取系统时间，单位ms
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
//获取系统时间，单位us
uint32_t micros(void)
{
  return getCurrentMicros();
}




