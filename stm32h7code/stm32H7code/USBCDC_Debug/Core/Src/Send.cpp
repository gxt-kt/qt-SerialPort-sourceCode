#include "Send.h"
#include "usbd_cdc_if.h"//ʹ��usb-cdc��������ļ�
#include "stdarg.h"

static uint8_t gSerialSendBuf[150];
//һ֡��ʼʱ�����Ĳ�������¼��֡ͷ����Ϣ
static void __gOneFrameStartHandle(uint8_t *str,uint8_t ch,uint8_t *cnt)
{
	str[(*cnt)++] = 0xAA;			  //֡ͷ
	str[(*cnt)++] = 0xFF;			  //֡ͷ
	str[(*cnt)++] = ch; //ͨ��
	str[(*cnt)++] = 0;			  //�����ֽ���
}
//һ֡����ʱ�����Ĳ�����У��+����
static void __gOneFrameEndHandle(uint8_t *str,uint8_t *cnt)
{
	uint8_t gxt_sc = 0;	//�����������ۼ�
	uint8_t gxt_ac = 0;	// ac��sc�����ۼ�
	str[3] = (*cnt) - 4;
	for (uint8_t gxt_i = 0; gxt_i < str[3] + 4; gxt_i++)
	{
		gxt_sc += str[gxt_i];
		gxt_ac += gxt_sc;
	}
	str[(*cnt)++] = gxt_sc;
	str[(*cnt)++] = gxt_ac;
	/********����Ĳ�Ҫ��***************/
	
	
#if 1 //ʹ��USB-CDC����
//FIXME:ʹ��usbcdc��Ҫ��CDC_Transmit_FS������ͷ�������´���
/************************************************************
  USBD_CDC_HandleTypeDef *ghcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;// ��ȡCDC״̬
	long long	int Tickstart = HAL_GetTick();		// ��ȡ��ǰʱ��
  while(ghcdc->TxState !=0)		// �ȴ��������
  {
    if((HAL_GetTick() - Tickstart) > 10){break;} 	// �����������ʱ��10ms
  }
*************************************************************/
    CDC_Transmit_FS((uint8_t*)str,(*cnt)); // ���� USB CDC������������
#elif
    HAL_UART_Transmit(&SEND_USART,(uint8_t *)str,cnt,0xFFFF);//hal�ⷢ������
#endif
}


void gSerialScopeCint(uint8_t ch,int a,int b,int c,int d,int e)
{
	uint8_t gxt_cnt = 0; //����
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
	uint8_t gxt_cnt = 0; //����
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
	uint8_t gxt_cnt = 0; //����
	__gOneFrameStartHandle(gSerialSendBuf,G_SERIAL_Show_Str,&gxt_cnt);

	gSerialSendBuf[gxt_cnt++] = color; //�����ֽ���
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
	uint8_t gxt_cnt = 0; //����

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
	uint8_t gxt_cnt = 0; //����
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
	uint8_t gxt_cnt = 0; //����
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
	uint8_t gxt_cnt = 0; //����
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
#if EnCppScope //�Ƿ�ʹ��cpp��ʾ������ֵ
/**
 * @brief ����ʾ����
 * @arg   gScopeBuf[0]: ����λ���ڴ������Ч����������Ĭ�Ϸ�Χ1-5
 * 						����λ���ڴ���������������ͣ�intΪ0��floatΪ1
 * 			ʾ����gScopeBuf[0]=0B01101011;
 * 			'011'��˼������Ч����Ϊ3���� '01011'���ǵ�һ������int��2float��3int��4float��5float
 * @arg gScopeBuf[1-20]: �ܹ�20������ĸ�����
 */
uint8_t gScopeBuf[21];
/**
 * @brief ���ٱȽϲ������ͣ��ɺ�gType(data)����
 * 
 * @param str1 ����typeid(data).name()
 * @param str2 ����"f"-float or "d"-double
 * @return true ȷ��Ϊfloat
 * @return false ȷ��Ϊint
 */
bool gScopeBufcmp(const char *str1, const char *str2)
{
	if (*str1 == *str2) // equal return1
		return 1;
	else
		return 0; // not equal return1
}

/**
 * @brief ��¼����//����Ҫ�û�������
 * 		
 * @param ch ��ǰͨ����V1.21�汾��λ��Ĭ�Ϸ�Χ1-4
 * @param dataN ʵ����Ч���ݸ���
 */
void __gxtSendScopeCpp(uint8_t ch, uint8_t dataN)
{
	uint8_t gxt_cnt = 0; //����
	__gOneFrameStartHandle(gSerialSendBuf,0xF0+ch,&gxt_cnt);

	gScopeBuf[0] = gScopeBuf[0] | (dataN << 5);
	for (uint8_t cnti = 0; cnti < 21; cnti++)
	{
		gSerialSendBuf[gxt_cnt++] = gScopeBuf[cnti];
	}

	__gOneFrameEndHandle(gSerialSendBuf,&gxt_cnt);

	gScopeBuf[0] = 0;//����׸���־λ
}
#endif //EnCppScope
//*****************************************
