#ifndef GXT_KT_GPIO_H__
#define GXT_KT_GPIO_H__

#include "my_sys.h"

class LED_SINGLE
{
public:
  //初始化led，GPIO:gpiox，GPIO_Pin：gpioPin，i：0共阴，1共阳
  LED_SINGLE(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin,bool i){
    GPIOx=GPIO;
    GPIO_Pin_x=GPIO_Pin;
    Blinkstandard=i;
  }
  void setSta(bool i);
  void open();
  void close();
  void toggle();
  void setBlinkStandard(bool i);
  bool getCurrentState();
  void setPin(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin);
private:
  GPIO_TypeDef *GPIOx;
  uint16_t GPIO_Pin_x;
  bool Blinkstandard;
};

//led颜色空间
namespace LED {
    extern bool black[];
    extern bool white[];
    extern bool red[];
    extern bool green[];
    extern bool blue[];
    extern bool orange[];
    extern bool yellow[];
    extern bool cyan[];
    extern bool magenta[];
}

//3色RGBled
class LED_RGB
{
public:
  //R-G-B-共阴共阳，0共阴，1共阳
  LED_RGB(GPIO_TypeDef *GPIO_R,uint16_t GPIO_Pin_R,GPIO_TypeDef *GPIO_G,\
  uint16_t GPIO_Pin_G,GPIO_TypeDef *GPIO_B,uint16_t GPIO_Pin_B,bool i)
  {
    ledR=new LED_SINGLE(GPIO_R,GPIO_Pin_R,i);
    ledG=new LED_SINGLE(GPIO_G,GPIO_Pin_G,i);
    ledB=new LED_SINGLE(GPIO_B,GPIO_Pin_B,i);
  }
  //设置颜色
  void setColor(bool* color);
private:
  LED_SINGLE *ledR;
  LED_SINGLE *ledG;
  LED_SINGLE *ledB;
};

#endif // !GXT_KT_GPIO_H__