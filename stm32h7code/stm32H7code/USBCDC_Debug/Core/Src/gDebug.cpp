#include "gDebug.h"
#include "stdarg.h"
#include "ringfifo.h"
#include "usbd_cdc_if.h"

static char gDebugBuf[256] = {0};//发送缓冲区

extern gRingFifo cdcFifo;
//用户更改实现方式，0串口直接堵塞发送，1usbcdc直接发送，2存入发送缓冲区
#define SendMethod 2
void gDebugSendString(const char *str, int num)
{
#if (SendMethod==0)//串口堵塞发送
  HAL_UART_Transmit(&SEND_USART,(uint8_t *)gDebugBuf,i,0xFFFF);//hal库发送数据

#elif (SendMethod==1)//USBCDC发送
  //FIXME:使用usbcdc需要在CDC_Transmit_FS函数开头加入以下代码
/************************************************************
  USBD_CDC_HandleTypeDef *ghcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;// 获取CDC状态
	long long	int Tickstart = HAL_GetTick();		// 获取当前时间
  while(ghcdc->TxState !=0)		// 等待发送完成
  {
    if((HAL_GetTick() - Tickstart) > 10){break;} 	// 设置阻塞最大时长10ms
  }
*************************************************************/
  CDC_Transmit_FS((uint8_t*)str,num); // 调用 USB CDC函数发送数据

#elif (SendMethod==2)//存入发送缓冲区
  //Ring_Fifo_Write_String(cdcFifo.getHandle(),(uint8_t*)str,num);
  cdcFifo.push((uint8_t *)str,num);
#endif
}

void gDebug(const char* fmt, ...)
{
#ifdef G_NO_DEBUG_OUTPUT //是否禁用调试功能
    return;
#endif // G_NO_DEBUG_OUTPUT
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)gDebugBuf, fmt, ap);
    va_end(ap);
    uint16_t i = strlen((const char *)gDebugBuf); //此次发送数据的长度
    gDebugSendString(gDebugBuf, i);
}


/**********C++版本，重载<<版本**************************************************************************************/
#if G_DEBUG_CPP_EN
static gxtCout gcout={};//实现自己的cout
struct gxtEndl {}endl;//定义结构体，实现对于endl特殊识别

gxtCout& gDebug(void)//函数重载
{
    return gcout;
}
#endif