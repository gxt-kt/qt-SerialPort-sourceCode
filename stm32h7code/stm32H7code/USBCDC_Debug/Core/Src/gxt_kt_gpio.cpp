#include "gxt_kt_gpio.h"

//����led��1�򿪣�0�ر�
void LED_SINGLE::setSta(bool i)
{
    if(i==0) close();
    else open();
}

//��led
void LED_SINGLE::open()
{
    if (Blinkstandard == 0)
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin_x, (GPIO_PinState)1);
    else
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin_x, (GPIO_PinState)0);
}

//�ر�led
void LED_SINGLE::close()
{
    if (Blinkstandard == 0)
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin_x, (GPIO_PinState)0);
    else
        HAL_GPIO_WritePin(GPIOx, GPIO_Pin_x, (GPIO_PinState)1);
}

//��תled
void LED_SINGLE::toggle()
{
    HAL_GPIO_TogglePin(GPIOx,GPIO_Pin_x);
}

//��ȡ��ǰ����������,1��0��
bool LED_SINGLE::getCurrentState()
{
    if(Blinkstandard==0) return ((bool)HAL_GPIO_ReadPin(GPIOx,GPIO_Pin_x));
    else return (!(bool)HAL_GPIO_ReadPin(GPIOx,GPIO_Pin_x));
}

//���ù���������0������1����
void LED_SINGLE::setBlinkStandard(bool i)
{
    Blinkstandard = i;
}

//������
void LED_SINGLE::setPin(GPIO_TypeDef *GPIO,uint16_t GPIO_Pin)
{
    GPIOx=GPIO;
    GPIO_Pin_x=GPIO_Pin;
}



namespace LED {
    bool black[3]={0,0,0};
    bool white[3]={1,1,1};
    bool red[3]={1,0,0};
    bool green[3]={0,1,0};
    bool blue[3]={0,0,1};
    bool yellow[3]={1,1,0};
    bool cyan[3]={0,1,1};
    bool magenta[3]={1,0,1};
}

void LED_RGB::setColor(bool* color){
    this->ledR->setSta(color[0]);
    this->ledG->setSta(color[1]);
    this->ledB->setSta(color[2]);
}