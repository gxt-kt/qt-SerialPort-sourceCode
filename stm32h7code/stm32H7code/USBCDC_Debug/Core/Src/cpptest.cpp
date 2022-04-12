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

	//��ʼ��PC13led
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
	//������ȿ�ʼ
  Scheduler_Setup();
}

void loop(void){
  Scheduler_Run();
}


static void Loop_1000Hz(void)	//1msִ��һ��
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

static void Loop_500Hz(void)	//2msִ��һ��
{	

}

static void Loop_200Hz(void)	//5msִ��һ��
{
  //static int ii=0;
  //ii++;
	//gDebug("testfifo%d\n",ii);
}

static void Loop_100Hz(void)	//10msִ��һ��
{
			//	if(Ring_Fifo_Get_Lenght(cdcFifo.getHandle())!=0)
  
}

static void Loop_50Hz(void)	//20msִ��һ��
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

static void Loop_20Hz(void)	//50msִ��һ��
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

static void Loop_2Hz(void)	//500msִ��һ��
{
  myled1.toggle();
}

static void Loop_1Hz(void)	//500msִ��һ��
{

}

typedef struct
{
void(*task_func)(void);
uint16_t rate_hz;
uint16_t interval_ticks;
uint32_t last_run;
}sched_task_t;

//ϵͳ�������ã�������ִͬ��Ƶ�ʵġ��̡߳�
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

//�������鳤�ȣ��ж��߳�����
#define TASK_NUM (sizeof(sched_tasks)/sizeof(sched_task_t))

//����ÿ���ʱ��
#define TICK_PER_SECOND	1000

//��ʼ��
void Scheduler_Setup(void)
{
	uint8_t index = 0;
	//��ʼ�������
	for(index=0;index < TASK_NUM;index++)
	{
		//����ÿ���������ʱ������
		sched_tasks[index].interval_ticks = TICK_PER_SECOND/sched_tasks[index].rate_hz;
		//�������Ϊ1��Ҳ����1ms
		if(sched_tasks[index].interval_ticks < 1)
		{
			sched_tasks[index].interval_ticks = 1;
		}
	}
}


/**
 * @brief ��������ŵ�main������while(1)�У���ͣ�ж��Ƿ����߳�Ӧ��ִ��
 * 
 */
void Scheduler_Run(void)
{
	uint8_t index = 0;
	//ѭ���ж������̣߳��Ƿ�Ӧ��ִ��
	for(index=0;index < TASK_NUM;index++)
	{
		//��ȡϵͳ��ǰʱ�䣬��λMS
		uint32_t tnow = millis();
		//�����жϣ������ǰʱ���ȥ��һ��ִ�е�ʱ�䣬���ڵ��ڸ��̵߳�ִ�����ڣ���ִ���߳�
		if(tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks)
		{
			//�����̵߳�ִ��ʱ�䣬������һ���ж�
			sched_tasks[index].last_run = tnow;
			//ִ���̺߳�����ʹ�õ��Ǻ���ָ��
			sched_tasks[index].task_func();
		}	 
	}
}