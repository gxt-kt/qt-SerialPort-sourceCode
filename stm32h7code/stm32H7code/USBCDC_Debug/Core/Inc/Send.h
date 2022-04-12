#ifndef Send_H__
#define Send_H__

#include "my_sys.h"

//************************************************************//
/**
 * @brief API!!!!
 * 
 */
//发送示波器值-C语言版的,这两个是示例demo，可以自己修改
void gSerialScopeCint(uint8_t ch,int a,int b,int c,int d,int e);
void gSerialScopeCfloat(uint8_t ch,float a,float b,float c,float d,float e);
//发送示波器值-C++语言版的
//void gSendScopeData(uint8_t ch,TA a,TB b,TC c,TD d,TE e);
//发送字符串，带颜色
void gSerialShowStr(uint8_t color, char *fmt, ...);
//发送欧拉角
void gSerialEulerPoint(float roll, float pitch, float yaw);
//发送四元数
void gSerialQuaternion(float p0, float p1, float p2, float p3);
//发送遥控器通道值
void gSerialRemoteControl(int16_t ch1, int16_t ch2, int16_t ch3, int16_t ch4, int16_t ch5);
//发送电压电流
void gSerialVoltageCurrent(float voltage, float current);

//************************************************************//


//定义颜色-用于传输尚上位机字符串显示颜色
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
#define EnCppScope 1 //是否使能cpp版示波器传值
#if EnCppScope //是否使能cpp版示波器传值

#include "typeinfo"//MDK文件，默认使用cpp是不需要额外文件的，arm操作需要包含这个文件//路径：D:\MDK_KEIL\ARM\ARMCC\include\typeinfo
extern uint8_t gScopeBuf[];//不需要用户操作，本来不想当全局变量，但是C++语法方便调用模板没办法，后面可能会更改
bool gScopeBufcmp(const char *str1,const char *str2);//不需要用户操作，
void __gxtSendScopeCpp(uint8_t ch,uint8_t dataN);//不需要用户操作，
#define gType(data) (gScopeBufcmp(typeid(data).name(),"f")||gScopeBufcmp(typeid(data).name(),"d"))//float or double return1,else return 0//不需要用户操作，

/**
 * @brief 使用C++检测变量类型，并写入到gScopeBuf内
 *        不需要用户操作，
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
 * @brief C++模板操作+函数重载-自适应
 *          需要用户调用，
 * @tparam TA-TE 模板自适应类型
 * @param ch :通道，V1.21版本上位机默认1-4
 * @param abcde :传入值
 *      示例：{
 *              int a=0;float b=2.3;
 *              gxtSendScope(1,a,b,1,2.0f);//发送通道1
 *              }
 * 
 */
template<class TA,class TB,class TC,class TD,class TE>
void gSerialScopeAuto(uint8_t ch,TA a,TB b,TC c,TD d,TE e){
    gxtSendScope(ch,a,b,c,d,e);
    __gxtSendScopeCpp(ch,5);
}
//注释同上。
template<class TA,class TB,class TC,class TD>
void gSerialScopeAuto(uint8_t ch,TA a,TB b,TC c,TD d){
    gxtSendScope(ch,a,b,c,d,0);
    __gxtSendScopeCpp(ch,4);
}
//注释同上。
template<class TA,class TB,class TC>
void gSerialScopeAuto(uint8_t ch,TA a,TB b,TC c){
    gxtSendScope(ch,a,b,c,0,0);
    __gxtSendScopeCpp(ch,3);
}
//注释同上。
template<class TA,class TB>
void gSerialScopeAuto(uint8_t ch,TA a,TB b){
    gxtSendScope(ch,a,b,0,0,0);
    __gxtSendScopeCpp(ch,2);
}
//注释同上。
template<class TA>
void gSerialScopeAuto(uint8_t ch,TA a){
    gxtSendScope(ch,a,0,0,0,0);
    __gxtSendScopeCpp(ch,1);
}
#endif //EnCppScope //是否使能cpp版示波器传值
//***************************************************


#endif // !Send_H__

