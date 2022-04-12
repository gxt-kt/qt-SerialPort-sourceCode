#include "gDebug.h"
#include "stdarg.h"
#include "ringfifo.h"
#include "usbd_cdc_if.h"

static char gDebugBuf[256] = {0};//���ͻ�����

extern gRingFifo cdcFifo;
//�û�����ʵ�ַ�ʽ��0����ֱ�Ӷ������ͣ�1usbcdcֱ�ӷ��ͣ�2���뷢�ͻ�����
#define SendMethod 2
void gDebugSendString(const char *str, int num)
{
#if (SendMethod==0)//���ڶ�������
  HAL_UART_Transmit(&SEND_USART,(uint8_t *)gDebugBuf,i,0xFFFF);//hal�ⷢ������

#elif (SendMethod==1)//USBCDC����
  //FIXME:ʹ��usbcdc��Ҫ��CDC_Transmit_FS������ͷ�������´���
/************************************************************
  USBD_CDC_HandleTypeDef *ghcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;// ��ȡCDC״̬
	long long	int Tickstart = HAL_GetTick();		// ��ȡ��ǰʱ��
  while(ghcdc->TxState !=0)		// �ȴ��������
  {
    if((HAL_GetTick() - Tickstart) > 10){break;} 	// �����������ʱ��10ms
  }
*************************************************************/
  CDC_Transmit_FS((uint8_t*)str,num); // ���� USB CDC������������

#elif (SendMethod==2)//���뷢�ͻ�����
  //Ring_Fifo_Write_String(cdcFifo.getHandle(),(uint8_t*)str,num);
  cdcFifo.push((uint8_t *)str,num);
#endif
}

void gDebug(const char* fmt, ...)
{
#ifdef G_NO_DEBUG_OUTPUT //�Ƿ���õ��Թ���
    return;
#endif // G_NO_DEBUG_OUTPUT
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)gDebugBuf, fmt, ap);
    va_end(ap);
    uint16_t i = strlen((const char *)gDebugBuf); //�˴η������ݵĳ���
    gDebugSendString(gDebugBuf, i);
}


/**********C++�汾������<<�汾**************************************************************************************/
#if G_DEBUG_CPP_EN
static gxtCout gcout={};//ʵ���Լ���cout
struct gxtEndl {}endl;//����ṹ�壬ʵ�ֶ���endl����ʶ��

gxtCout& gDebug(void)//��������
{
    return gcout;
}
#endif