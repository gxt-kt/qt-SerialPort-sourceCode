#include "cpptest.h"
#include "gDebug.h"
#include "gxt_kt_gpio.h"
#include "ringfifo.h"
#include "usbd_cdc_if.h"
#include "Send.h"

void Scheduler_Setup(void);
void Scheduler_Run(void);


gRingFifo cdcFifo(256);

LED_SINGLE myled1(GPIOC,GPIO_PIN_13,1);

void setup(void){

	//初始化PC13led
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
	//任务调度开始
  Scheduler_Setup();
}

void loop(void){
  Scheduler_Run();
}


static void Loop_1000Hz(void)	//1ms执行一次
{
//	uint32_t cdcLength=cdcFifo.getUsedNum();
//  if(cdcLength)
//  {
//    //gDebug()<< "length" << cdcLength<<endl;
//    uint8_t str[256]={0};
//    cdcFifo.pop(str,cdcLength);
//    CDC_Transmit_FS(str,cdcLength);
//  }
}

static void Loop_500Hz(void)	//2ms执行一次
{	

}

static void Loop_200Hz(void)	//5ms执行一次
{
  //static int ii=0;
  //ii++;
	//gDebug("testfifo%d\n",ii);
}

static void Loop_100Hz(void)	//10ms执行一次
{
			//	if(Ring_Fifo_Get_Lenght(cdcFifo.getHandle())!=0)
  
}

static void Loop_50Hz(void)	//20ms执行一次
{	
	static int num;
	static float fi;
	fi+=0.1;
	gSerialShowStr(gxtORANGE,"test%d",num++);
	gSerialEulerPoint(10,20,30);
	gSerialQuaternion(0.001,0.002,0.003,0.004);
	gSerialRemoteControl(1100,1200,1300,1400,1500);
	gSerialScopeAuto(1,sin(fi),cos(fi));
}

static void Loop_20Hz(void)	//50ms执行一次
{	
  //gDebug()<< SysTick->VAL << " "<< SysTick->LOAD;
  //gDebug()<< " sysclock=" << SystemCoreClock<<endl;
  //gDebug()<< SysTick->VAL<<"   ";
  //gDebug("%d   ",SysTick->VAL);
//  gDebug()<<micros()<<" "<<millis()<<"  ";
//  HAL_TimDelay_us(251);
//  gDebug()<<micros()<<" "<<millis()<<"  "<<endl;
  //gDebug("%d\n",SysTick->VAL);
}

static void Loop_2Hz(void)	//500ms执行一次
{
  myled1.toggle();
}

static void Loop_1Hz(void)	//500ms执行一次
{

}

typedef struct
{
void(*task_func)(void);
uint16_t rate_hz;
uint16_t interval_ticks;
uint32_t last_run;
}sched_task_t;

//系统任务配置，创建不同执行频率的“线程”
static sched_task_t sched_tasks[] = 
{
	{Loop_1000Hz,1000,  0, 0},
	{Loop_500Hz , 500,  0, 0},
	{Loop_200Hz , 200,  0, 0},
	{Loop_100Hz , 100,  0, 0},
	{Loop_50Hz  ,  50,  0, 0},
	{Loop_20Hz  ,  20,  0, 0},
	{Loop_2Hz   ,   2,  0, 0},
  {Loop_1Hz   ,   1,  0, 0},
};

//根据数组长度，判断线程数量
#define TASK_NUM (sizeof(sched_tasks)/sizeof(sched_task_t))

//定义每秒计时数
#define TICK_PER_SECOND	1000

//初始化
void Scheduler_Setup(void)
{
	uint8_t index = 0;
	//初始化任务表
	for(index=0;index < TASK_NUM;index++)
	{
		//计算每个任务的延时周期数
		sched_tasks[index].interval_ticks = TICK_PER_SECOND/sched_tasks[index].rate_hz;
		//最短周期为1，也就是1ms
		if(sched_tasks[index].interval_ticks < 1)
		{
			sched_tasks[index].interval_ticks = 1;
		}
	}
}


/**
 * @brief 这个函数放到main函数的while(1)中，不停判断是否有线程应该执行
 * 
 */
void Scheduler_Run(void)
{
	uint8_t index = 0;
	//循环判断所有线程，是否应该执行
	for(index=0;index < TASK_NUM;index++)
	{
		//获取系统当前时间，单位MS
		uint32_t tnow = millis();
		//进行判断，如果当前时间减去上一次执行的时间，大于等于该线程的执行周期，则执行线程
		if(tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{
			//更新线程的执行时间，用于下一次判断
			sched_tasks[index].last_run = tnow;
			//执行线程函数，使用的是函数指针
			sched_tasks[index].task_func();
		}	 
	}
}