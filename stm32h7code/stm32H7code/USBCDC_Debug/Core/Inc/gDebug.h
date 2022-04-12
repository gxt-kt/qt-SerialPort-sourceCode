#ifndef GDEBUG_H__
#define GDEBUG_H__

#include "my_sys.h"

//CPP����
#ifdef __cplusplus

    #define G_DEBUG_CPP_EN 1    //�Ƿ�֧��cpp (cout << endl)

#endif



/************************************************************************/
//���Է��ͺ���
void gDebug(const char *fmt, ...); // C�汾

#if G_DEBUG_CPP_EN
struct gxtEndl;             //�����ṹ��
extern struct gxtEndl endl; //�������ļ���ʹ��endl
class gxtCout               //ʵ��<<���������
{
public:
    gxtCout &operator<<(gxtEndl &gendl){
        gDebug("\r\n");
        return *this;
    }
    gxtCout &operator<<(char c){
        gDebug("%c", c);
        return *this;
    }
    gxtCout &operator<<(const char *str){
        gDebug("%s", str);
        return *this;
    }
    gxtCout &operator<<(short number){
        gDebug("%d", number);
        return *this;
    }
    gxtCout &operator<<(int number){
        gDebug("%d", number);
        return *this;
    }
    gxtCout &operator<<(uint32_t number){
        gDebug("%d", number);
        return *this;
    }
    gxtCout &operator<<(uint16_t number){
        gDebug("%d", number);
        return *this;
    }
    gxtCout &operator<<(uint8_t number){
        gDebug("%d", number);
        return *this;
    }
    gxtCout &operator<<(uint8_t *string)
    {
        gDebug("%s", string);
        return *this;
    }
    gxtCout &operator<<(char *string)
    {
        gDebug("%s", string);
        return *this;
    }
    gxtCout &operator<<(long number)
    {
        gDebug("%d", number);
        return *this;
    }
    gxtCout &operator<<(float number){
        gDebug("%f", number);
        return *this;
    }
    gxtCout &operator<<(double number){
        gDebug("%f", number);
        return *this;
    }
};
gxtCout &gDebug(void); //���غ���cpp�汾
#endif                 // DEBUG-CPP

/****************private*************************************/

#endif // !GDEBUG_H__