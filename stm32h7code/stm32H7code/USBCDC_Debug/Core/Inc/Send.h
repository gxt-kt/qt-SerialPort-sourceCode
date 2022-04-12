#ifndef Send_H__
#define Send_H__

#include "my_sys.h"

//************************************************************//
/**
 * @brief API!!!!
 * 
 */
//����ʾ����ֵ-C���԰��,��������ʾ��demo�������Լ��޸�
void gSerialScopeCint(uint8_t ch,int a,int b,int c,int d,int e);
void gSerialScopeCfloat(uint8_t ch,float a,float b,float c,float d,float e);
//����ʾ����ֵ-C++���԰��
//void gSendScopeData(uint8_t ch,TA a,TB b,TC c,TD d,TE e);
//�����ַ���������ɫ
void gSerialShowStr(uint8_t color, char *fmt, ...);
//����ŷ����
void gSerialEulerPoint(float roll, float pitch, float yaw);
//������Ԫ��
void gSerialQuaternion(float p0, float p1, float p2, float p3);
//����ң����ͨ��ֵ
void gSerialRemoteControl(int16_t ch1, int16_t ch2, int16_t ch3, int16_t ch4, int16_t ch5);
//���͵�ѹ����
void gSerialVoltageCurrent(float voltage, float current);

//************************************************************//


//������ɫ-���ڴ�������λ���ַ�����ʾ��ɫ
typedef enum
{
    gxtBLACK = 0U,
    gxtWHITE,
    gxtRED,
    gxtGREEN,
    gxtBLUE,
    gxtORANGE,
    gxtYELLOW,
    gxtCYAN,
    gxtMagenta,
} gxtCOLOR;

#define BYTE0(dwTemp) (*((char *)(&dwTemp)))
#define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

//***************************************************
#define EnCppScope 1 //�Ƿ�ʹ��cpp��ʾ������ֵ
#if EnCppScope //�Ƿ�ʹ��cpp��ʾ������ֵ

#include "typeinfo"//MDK�ļ���Ĭ��ʹ��cpp�ǲ���Ҫ�����ļ��ģ�arm������Ҫ��������ļ�//·����D:\MDK_KEIL\ARM\ARMCC\include\typeinfo
extern uint8_t gScopeBuf[];//����Ҫ�û��������������뵱ȫ�ֱ���������C++�﷨�������ģ��û�취��������ܻ����
bool gScopeBufcmp(const char *str1,const char *str2);//����Ҫ�û�������
void __gxtSendScopeCpp(uint8_t ch,uint8_t dataN);//����Ҫ�û�������
#define gType(data) (gScopeBufcmp(typeid(data).name(),"f")||gScopeBufcmp(typeid(data).name(),"d"))//float or double return1,else return 0//����Ҫ�û�������

/**
 * @brief ʹ��C++���������ͣ���д�뵽gScopeBuf��
 *        ����Ҫ�û�������
 */
#define gxtSendScope(ch,a,b,c,d,e) {\
    uint16_t gScopeCnt=1;           \
    if(gType(a)==1) {float A=a;gScopeBuf[gScopeCnt++]=BYTE0(A);gScopeBuf[gScopeCnt++]=BYTE1(A);gScopeBuf[gScopeCnt++]=BYTE2(A);gScopeBuf[gScopeCnt++]=BYTE3(A);}\
    else {int A=a;gScopeBuf[gScopeCnt++]=BYTE0(A);gScopeBuf[gScopeCnt++]=BYTE1(A);gScopeBuf[gScopeCnt++]=BYTE2(A);gScopeBuf[gScopeCnt++]=BYTE3(A);}             \
                                    \
    if(gType(b)==1) {float B=b;gScopeBuf[gScopeCnt++]=BYTE0(B);gScopeBuf[gScopeCnt++]=BYTE1(B);gScopeBuf[gScopeCnt++]=BYTE2(B);gScopeBuf[gScopeCnt++]=BYTE3(B);}\
    else {int B=b;gScopeBuf[gScopeCnt++]=BYTE0(B);gScopeBuf[gScopeCnt++]=BYTE1(B);gScopeBuf[gScopeCnt++]=BYTE2(B);gScopeBuf[gScopeCnt++]=BYTE3(B);}   \
                                    \
    if(gType(c)==1) {float C=c;gScopeBuf[gScopeCnt++]=BYTE0(C);gScopeBuf[gScopeCnt++]=BYTE1(C);gScopeBuf[gScopeCnt++]=BYTE2(C);gScopeBuf[gScopeCnt++]=BYTE3(C);}\
    else {int C=c;gScopeBuf[gScopeCnt++]=BYTE0(C);gScopeBuf[gScopeCnt++]=BYTE1(C);gScopeBuf[gScopeCnt++]=BYTE2(C);gScopeBuf[gScopeCnt++]=BYTE3(C);}   \
                                    \
    if(gType(d)==1) {float D=d;gScopeBuf[gScopeCnt++]=BYTE0(D);gScopeBuf[gScopeCnt++]=BYTE1(D);gScopeBuf[gScopeCnt++]=BYTE2(D);gScopeBuf[gScopeCnt++]=BYTE3(D);}\
    else {int D=d;gScopeBuf[gScopeCnt++]=BYTE0(D);gScopeBuf[gScopeCnt++]=BYTE1(D);gScopeBuf[gScopeCnt++]=BYTE2(D);gScopeBuf[gScopeCnt++]=BYTE3(D);}    \
                                    \
    if(gType(e)==1) {float E=e;gScopeBuf[gScopeCnt++]=BYTE0(E);gScopeBuf[gScopeCnt++]=BYTE1(E);gScopeBuf[gScopeCnt++]=BYTE2(E);gScopeBuf[gScopeCnt++]=BYTE3(E);}\
    else {int E=e;gScopeBuf[gScopeCnt++]=BYTE0(E);gScopeBuf[gScopeCnt++]=BYTE1(E);gScopeBuf[gScopeCnt++]=BYTE2(E);gScopeBuf[gScopeCnt++]=BYTE3(E);}   \
                                    \
     gScopeBuf[0]=(gType(a)*16)|(gType(b)*8)|(gType(c)*4)|(gType(d)*2)|(gType(e)*1);\
}


/**
 * @brief C++ģ�����+��������-����Ӧ
 *          ��Ҫ�û����ã�
 * @tparam TA-TE ģ������Ӧ����
 * @param ch :ͨ����V1.21�汾��λ��Ĭ��1-4
 * @param abcde :����ֵ
 *      ʾ����{
 *              int a=0;float b=2.3;
 *              gxtSendScope(1,a,b,1,2.0f);//����ͨ��1
 *              }
 * 
 */
template<class TA,class TB,class TC,class TD,class TE>
void gSerialScopeAuto(uint8_t ch,TA a,TB b,TC c,TD d,TE e){
    gxtSendScope(ch,a,b,c,d,e);
    __gxtSendScopeCpp(ch,5);
}
//ע��ͬ�ϡ�
template<class TA,class TB,class TC,class TD>
void gSerialScopeAuto(uint8_t ch,TA a,TB b,TC c,TD d){
    gxtSendScope(ch,a,b,c,d,0);
    __gxtSendScopeCpp(ch,4);
}
//ע��ͬ�ϡ�
template<class TA,class TB,class TC>
void gSerialScopeAuto(uint8_t ch,TA a,TB b,TC c){
    gxtSendScope(ch,a,b,c,0,0);
    __gxtSendScopeCpp(ch,3);
}
//ע��ͬ�ϡ�
template<class TA,class TB>
void gSerialScopeAuto(uint8_t ch,TA a,TB b){
    gxtSendScope(ch,a,b,0,0,0);
    __gxtSendScopeCpp(ch,2);
}
//ע��ͬ�ϡ�
template<class TA>
void gSerialScopeAuto(uint8_t ch,TA a){
    gxtSendScope(ch,a,0,0,0,0);
    __gxtSendScopeCpp(ch,1);
}
#endif //EnCppScope //�Ƿ�ʹ��cpp��ʾ������ֵ
//***************************************************


#endif // !Send_H__

