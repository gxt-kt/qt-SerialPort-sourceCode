#include "Send.h"
#include "usbd_cdc_if.h"//使用usb-cdc包含这个文件
#include "stdarg.h"

static uint8_t gSerialSendBuf[150];
//一帧开始时所做的操作，记录下帧头等信息
static void __gOneFrameStartHandle(uint8_t *str,uint8_t ch,uint8_t *cnt)
{
	str[(*cnt)++] = 0xAA;			  //帧头
	str[(*cnt)++] = 0xFF;			  //帧头
	str[(*cnt)++] = ch; //通道
	str[(*cnt)++] = 0;			  //数据字节数
}
//一帧结束时所做的操作，校验+发送
static void __gOneFrameEndHandle(uint8_t *str,uint8_t *cnt)
{
	uint8_t gxt_sc = 0;	//对总数进行累加
	uint8_t gxt_ac = 0;	// ac对sc进行累加
	str[3] = (*cnt) - 4;
	for (uint8_t gxt_i = 0; gxt_i < str[3] + 4; gxt_i++)
	{
		gxt_sc += str[gxt_i];
		gxt_ac += gxt_sc;
	}
	str[(*cnt)++] = gxt_sc;
	str[(*cnt)++] = gxt_ac;
	/********上面的不要动***************/
	
	
#if 1 //使用USB-CDC发送
//FIXME:使用usbcdc需要在CDC_Transmit_FS函数开头加入以下代码
/************************************************************
  USBD_CDC_HandleTypeDef *ghcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;// 获取CDC状态
	long long	int Tickstart = HAL_GetTick();		// 获取当前时间
  while(ghcdc->TxState !=0)		// 等待发送完成
  {
    if((HAL_GetTick() - Tickstart) > 10){break;} 	// 设置阻塞最大时长10ms
  }
*************************************************************/
    CDC_Transmit_FS((uint8_t*)str,(*cnt)); // 调用 USB CDC函数发送数据
#elif
    HAL_UART_Transmit(&SEND_USART,(uint8_t *)str,cnt,0xFFFF);//hal库发送数据
#endif
}


void gSerialScopeCint(uint8_t ch,int a,int b,int c,int d,int e)
{
	uint8_t gxt_cnt = 0; //计数
	__gOneFrameStartHandle(gSerialSendBuf,0xF0+ch,&gxt_cnt);
	gSerialSendBuf[gxt_cnt++] = BYTE0(a);
	gSerialSendBuf[gxt_cnt++] = BYTE1(a);
	gSerialSendBuf[gxt_cnt++] = BYTE2(a);
	gSerialSendBuf[gxt_cnt++] = BYTE3(a);
	gSerialSendBuf[gxt_cnt++] = BYTE0(b);
	gSerialSendBuf[gxt_cnt++] = BYTE1(b);
	gSerialSendBuf[gxt_cnt++] = BYTE2(b);
	gSerialSendBuf[gxt_cnt++] = BYTE3(b);
	gSerialSendBuf[gxt_cnt++] = BYTE0(c);
	gSerialSendBuf[gxt_cnt++] = BYTE1(c);
	gSerialSendBuf[gxt_cnt++] = BYTE2(c);
	gSerialSendBuf[gxt_cnt++] = BYTE3(c);
	gSerialSendBuf[gxt_cnt++] = BYTE0(d);
	gSerialSendBuf[gxt_cnt++] = BYTE1(d);
	gSerialSendBuf[gxt_cnt++] = BYTE2(d);
	gSerialSendBuf[gxt_cnt++] = BYTE3(d);
	gSerialSendBuf[gxt_cnt++] = BYTE0(e);
	gSerialSendBuf[gxt_cnt++] = BYTE1(e);
	gSerialSendBuf[gxt_cnt++] = BYTE2(e);
	gSerialSendBuf[gxt_cnt++] = BYTE3(e);
	__gOneFrameEndHandle(gSerialSendBuf,&gxt_cnt);
}

void gSerialScopeCfloat(uint8_t ch,float a,float b,float c,float d,float e)
{
	uint8_t gxt_cnt = 0; //计数
	__gOneFrameStartHandle(gSerialSendBuf,0xF0+ch,&gxt_cnt);
	gSerialSendBuf[gxt_cnt++] = BYTE0(a);
	gSerialSendBuf[gxt_cnt++] = BYTE1(a);
	gSerialSendBuf[gxt_cnt++] = BYTE2(a);
	gSerialSendBuf[gxt_cnt++] = BYTE3(a);
	gSerialSendBuf[gxt_cnt++] = BYTE0(b);
	gSerialSendBuf[gxt_cnt++] = BYTE1(b);
	gSerialSendBuf[gxt_cnt++] = BYTE2(b);
	gSerialSendBuf[gxt_cnt++] = BYTE3(b);
	gSerialSendBuf[gxt_cnt++] = BYTE0(c);
	gSerialSendBuf[gxt_cnt++] = BYTE1(c);
	gSerialSendBuf[gxt_cnt++] = BYTE2(c);
	gSerialSendBuf[gxt_cnt++] = BYTE3(c);
	gSerialSendBuf[gxt_cnt++] = BYTE0(d);
	gSerialSendBuf[gxt_cnt++] = BYTE1(d);
	gSerialSendBuf[gxt_cnt++] = BYTE2(d);
	gSerialSendBuf[gxt_cnt++] = BYTE3(d);
	gSerialSendBuf[gxt_cnt++] = BYTE0(e);
	gSerialSendBuf[gxt_cnt++] = BYTE1(e);
	gSerialSendBuf[gxt_cnt++] = BYTE2(e);
	gSerialSendBuf[gxt_cnt++] = BYTE3(e);
	__gOneFrameEndHandle(gSerialSendBuf,&gxt_cnt);
}
#define G_SERIAL_Show_Str 0xC0
void gSerialShowStr(uint8_t color, char *fmt, ...)
{
	uint8_t gxt_cnt = 0; //计数
	__gOneFrameStartHandle(gSerialSendBuf,G_SERIAL_Show_Str,&gxt_cnt);

	gSerialSendBuf[gxt_cnt++] = color; //数据字节数
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)(gSerialSendBuf + gxt_cnt), fmt, ap);
	va_end(ap);
	gxt_cnt = gxt_cnt + strlen((const char *)(gSerialSendBuf + gxt_cnt));

	__gOneFrameEndHandle(gSerialSendBuf,&gxt_cnt);
}

#define G_SERIAL_EulerPoint 0x03
void gSerialEulerPoint(float roll, float pitch, float yaw)
{
	uint8_t gxt_cnt = 0; //计数

	int16_t Roll = roll * 100;
	int16_t Pitch = pitch * 100;
	int16_t Yaw = yaw * 100;

	__gOneFrameStartHandle(gSerialSendBuf,G_SERIAL_EulerPoint,&gxt_cnt);

	gSerialSendBuf[gxt_cnt++] = BYTE0(Roll);
	gSerialSendBuf[gxt_cnt++] = BYTE1(Roll);
	gSerialSendBuf[gxt_cnt++] = BYTE0(Pitch);
	gSerialSendBuf[gxt_cnt++] = BYTE1(Pitch);
	gSerialSendBuf[gxt_cnt++] = BYTE0(Yaw);
	gSerialSendBuf[gxt_cnt++] = BYTE1(Yaw);
	
	__gOneFrameEndHandle(gSerialSendBuf,&gxt_cnt);
}

#define G_SERIAL_Quaternion 0x04
void gSerialQuaternion(float p0, float p1, float p2, float p3)
{
	uint8_t gxt_cnt = 0; //计数
	__gOneFrameStartHandle(gSerialSendBuf,G_SERIAL_Quaternion,&gxt_cnt);

	int16_t p0val = p0 * 10000;
	int16_t p1val = p1 * 10000;
	int16_t p2val = p2 * 10000;
	int16_t p3val = p3 * 10000;
	gSerialSendBuf[gxt_cnt++] = BYTE0(p0val);
	gSerialSendBuf[gxt_cnt++] = BYTE1(p0val);
	gSerialSendBuf[gxt_cnt++] = BYTE0(p1val);
	gSerialSendBuf[gxt_cnt++] = BYTE1(p1val);
	gSerialSendBuf[gxt_cnt++] = BYTE0(p2val);
	gSerialSendBuf[gxt_cnt++] = BYTE1(p2val);
	gSerialSendBuf[gxt_cnt++] = BYTE0(p3val);
	gSerialSendBuf[gxt_cnt++] = BYTE1(p3val);

	__gOneFrameEndHandle(gSerialSendBuf,&gxt_cnt);
}

#define G_SERIAL_RemoteControl 0x40
void gSerialRemoteControl(int16_t ch1, int16_t ch2, int16_t ch3, int16_t ch4, int16_t ch5)
{
	uint8_t gxt_cnt = 0; //计数
	__gOneFrameStartHandle(gSerialSendBuf,G_SERIAL_RemoteControl,&gxt_cnt);

	gSerialSendBuf[gxt_cnt++] = BYTE0(ch1);
	gSerialSendBuf[gxt_cnt++] = BYTE1(ch1);
	gSerialSendBuf[gxt_cnt++] = BYTE0(ch2);
	gSerialSendBuf[gxt_cnt++] = BYTE1(ch2);
	gSerialSendBuf[gxt_cnt++] = BYTE0(ch3);
	gSerialSendBuf[gxt_cnt++] = BYTE1(ch3);
	gSerialSendBuf[gxt_cnt++] = BYTE0(ch4);
	gSerialSendBuf[gxt_cnt++] = BYTE1(ch4);
	gSerialSendBuf[gxt_cnt++] = BYTE0(ch5);
	gSerialSendBuf[gxt_cnt++] = BYTE1(ch5);

	__gOneFrameEndHandle(gSerialSendBuf,&gxt_cnt);
}

#define G_SERIAL_VoltageCurrent 0x0D
void gSerialVoltageCurrent(float voltage, float current)
{
	uint8_t gxt_cnt = 0; //计数
	__gOneFrameStartHandle(gSerialSendBuf,G_SERIAL_VoltageCurrent,&gxt_cnt);

	uint16_t realSendVoltage = (uint16_t)(voltage * 100);
	uint16_t realSendCurrent = (uint16_t)(current * 100);
	gSerialSendBuf[gxt_cnt++] = BYTE0(realSendVoltage);
	gSerialSendBuf[gxt_cnt++] = BYTE1(realSendVoltage);
	gSerialSendBuf[gxt_cnt++] = BYTE0(realSendCurrent);
	gSerialSendBuf[gxt_cnt++] = BYTE1(realSendCurrent);

	__gOneFrameEndHandle(gSerialSendBuf,&gxt_cnt);
}

//*****************************************
#if EnCppScope //是否使能cpp版示波器传值
/**
 * @brief 用于示波器
 * @arg   gScopeBuf[0]: 高三位用于传输的有效参数个数，默认范围1-5
 * 						低五位用于代表五个参数的类型，int为0，float为1
 * 			示例：gScopeBuf[0]=0B01101011;
 * 			'011'意思就是有效参数为3个， '01011'就是第一个参数int，2float，3int，4float，5float
 * @arg gScopeBuf[1-20]: 总共20个存放四个参数
 */
uint8_t gScopeBuf[21];
/**
 * @brief 快速比较参数类型，由宏gType(data)调用
 * 
 * @param str1 传入typeid(data).name()
 * @param str2 传入"f"-float or "d"-double
 * @return true 确定为float
 * @return false 确定为int
 */
bool gScopeBufcmp(const char *str1, const char *str2)
{
	if (*str1 == *str2) // equal return1
		return 1;
	else
		return 0; // not equal return1
}

/**
 * @brief 记录数据//不需要用户操作，
 * 		
 * @param ch 当前通道，V1.21版本上位机默认范围1-4
 * @param dataN 实际有效数据个数
 */
void __gxtSendScopeCpp(uint8_t ch, uint8_t dataN)
{
	uint8_t gxt_cnt = 0; //计数
	__gOneFrameStartHandle(gSerialSendBuf,0xF0+ch,&gxt_cnt);

	gScopeBuf[0] = gScopeBuf[0] | (dataN << 5);
	for (uint8_t cnti = 0; cnti < 21; cnti++)
	{
		gSerialSendBuf[gxt_cnt++] = gScopeBuf[cnti];
	}

	__gOneFrameEndHandle(gSerialSendBuf,&gxt_cnt);

	gScopeBuf[0] = 0;//清空首个标志位
}
#endif //EnCppScope
//*****************************************
