#ifndef RINGFIFO_H__
#define RINGFIFO_H__

/**
 * \file ring_Fifo.h
 * \brief ���׻��λ�����ض���������
 * \author netube_99\netube@163.com
 * \date 2021.06.29
 * \version v1.3.2
*/


#include <stdint.h>
#include <string.h>

#define RING_FIFO_SUCCESS     0x01
#define RING_FIFO_ERROR       0x00

//���λ������ṹ��
typedef struct
{
    uint32_t head ;//����ͷָ��
    uint32_t tail ;//����βָ��
    uint32_t lenght ;//�Ѵ����������
    uint8_t *array_addr ;//�����������������ַ
    uint32_t max_lenght ;//���������ɴ���������
}ring_Fifo;


uint8_t Ring_Fifo_Init(ring_Fifo *ring_Fifo_handle, uint8_t *Fifo_addr ,uint32_t Fifo_size);//��ʼ���»�����
uint8_t Ring_Fifo_Delete(ring_Fifo *ring_Fifo_handle, uint32_t lenght);//��ͷָ�뿪ʼɾ��ָ�����ȵ�����
uint8_t Ring_Fifo_Write_Byte(ring_Fifo *ring_Fifo_handle, uint8_t data);//�򻺳�����дһ���ֽ�
uint8_t Ring_Fifo_Read_Byte(ring_Fifo *ring_Fifo_handle);//�ӻ�������ȡһ���ֽ�
uint8_t Ring_Fifo_Write_String(ring_Fifo *ring_Fifo_handle, uint8_t *input_addr, uint32_t write_lenght);//�򻺳�����дָ����������
uint8_t Ring_Fifo_Read_String(ring_Fifo *ring_Fifo_handle, uint8_t *output_addr, uint32_t read_lenght);//�ӻ�������ȡָ����������
uint8_t Ring_Fifo_Insert_Keyword(ring_Fifo *ring_Fifo_handle, uint32_t keyword, uint8_t keyword_lenght);//���λ���������ؼ���
uint32_t Ring_Fifo_Find_Keyword(ring_Fifo *ring_Fifo_handle, uint32_t keyword, uint8_t keyword_lenght);//��ͷָ�뿪ʼ���������ƥ���ַ�
static uint32_t Ring_Fifo_Get_Word(ring_Fifo *ring_Fifo_handle, uint32_t head, uint32_t read_lenght);//��ָ��ͷָ���ַ��ȡ�������ȵĹؼ��ʣ�˽�к�������ָ��Խλ������
uint32_t Ring_Fifo_Get_Lenght(ring_Fifo *ring_Fifo_handle);//��ȡ���������Ѵ�������ݳ���
uint32_t Ring_Fifo_Get_FreeSize(ring_Fifo *ring_Fifo_handle);//��ȡ���������ô���ռ�


//---------------------------------GXT�������-BEGIN-----------------------
//CPP����
#ifdef __cplusplus
    #define gRingFifo_En_Cpp 1    //�Ƿ�֧��cpp (cout << endl)
#endif

#if gRingFifo_En_Cpp//GXT-ʹ��C++������ʹ��
class gRingFifo
{
private:
    /* data */
    uint32_t num;
    uint8_t *ringFifoBuf;
    ring_Fifo *ringFifoHandle;
public:
    //��ʼ�����캯��
    gRingFifo(uint32_t size){
        num=size;
        ringFifoBuf=new uint8_t [size];//�ڶ���������Ҫ������
        ringFifoHandle=new ring_Fifo;
        Ring_Fifo_Init(ringFifoHandle,ringFifoBuf,size);
    }
    //��ȡ���
    ring_Fifo * getHandle(){
        return ringFifoHandle;
    }
    //��ȡ�洢buf�׵�ַ
    uint8_t * getBufAdd(){
        return ringFifoBuf;
    }
    //��ȡ����������
    uint32_t getWholeNum(){
        return num;
    }
    //��ȡ���������ó���
    uint32_t getUnusedNum(){
        return Ring_Fifo_Get_FreeSize(ringFifoHandle);
    }
    //��ȡ���������Ѵ�������ݳ���
    uint32_t getUsedNum(){
        return Ring_Fifo_Get_Lenght(ringFifoHandle);
    }
    //д��һ���ֽ�,����д��ɹ���ʧ��
    uint8_t push(uint8_t data){
        return Ring_Fifo_Write_Byte(ringFifoHandle,data);
    }
    //��ȡһ���ֽڣ����ض�ȡ���ֽ�
    uint8_t pop(){
        return Ring_Fifo_Read_Byte(ringFifoHandle);
    }
    //д��һ���ֽ�,����д��ɹ���ʧ��
    uint8_t push(uint8_t *address,uint32_t num){
        return Ring_Fifo_Write_String(ringFifoHandle,address,num);
    }
    //д��һ���ֽ�,����д��ɹ���ʧ��,��Ӧchar*����
    uint8_t push(char *address,uint32_t num){
        return Ring_Fifo_Write_String(ringFifoHandle,(uint8_t*)address,num);
    }
    //��ȡһ���ֽ�,����ΪҪ�洢�ĵ�ַ�Ͷ�ȡ���������ض�ȡ�ɹ���ʧ��
    uint8_t pop(uint8_t *address,uint32_t num){
        return Ring_Fifo_Read_String(ringFifoHandle,address,num);
    }
    //��ͷָ�뿪ʼɾ��ָ�����ȵ����ݣ�����ɾ���Ƿ�ɹ�
    uint8_t deleteNum(uint32_t delete_num){
        return Ring_Fifo_Delete(ringFifoHandle,delete_num);
    }
    ~gRingFifo(){
        delete ringFifoBuf;
        delete &num;
        delete ringFifoHandle;
    }
};
#endif
//---------------------------------GXT�������-END-----------------------


#endif // !RINGFIFO_H__
