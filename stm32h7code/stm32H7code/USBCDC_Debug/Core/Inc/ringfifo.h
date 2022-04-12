#ifndef RINGFIFO_H__
#define RINGFIFO_H__

/**
 * \file ring_Fifo.h
 * \brief 简易环形缓冲相关定义与声明
 * \author netube_99\netube@163.com
 * \date 2021.06.29
 * \version v1.3.2
*/


#include <stdint.h>
#include <string.h>

#define RING_FIFO_SUCCESS     0x01
#define RING_FIFO_ERROR       0x00

//环形缓冲区结构体
typedef struct
{
    uint32_t head ;//操作头指针
    uint32_t tail ;//操作尾指针
    uint32_t lenght ;//已储存的数据量
    uint8_t *array_addr ;//缓冲区储存数组基地址
    uint32_t max_lenght ;//缓冲区最大可储存数据量
}ring_Fifo;


uint8_t Ring_Fifo_Init(ring_Fifo *ring_Fifo_handle, uint8_t *Fifo_addr ,uint32_t Fifo_size);//初始化新缓冲区
uint8_t Ring_Fifo_Delete(ring_Fifo *ring_Fifo_handle, uint32_t lenght);//从头指针开始删除指定长度的数据
uint8_t Ring_Fifo_Write_Byte(ring_Fifo *ring_Fifo_handle, uint8_t data);//向缓冲区里写一个字节
uint8_t Ring_Fifo_Read_Byte(ring_Fifo *ring_Fifo_handle);//从缓冲区读取一个字节
uint8_t Ring_Fifo_Write_String(ring_Fifo *ring_Fifo_handle, uint8_t *input_addr, uint32_t write_lenght);//向缓冲区里写指定长度数据
uint8_t Ring_Fifo_Read_String(ring_Fifo *ring_Fifo_handle, uint8_t *output_addr, uint32_t read_lenght);//从缓冲区读取指定长度数据
uint8_t Ring_Fifo_Insert_Keyword(ring_Fifo *ring_Fifo_handle, uint32_t keyword, uint8_t keyword_lenght);//环形缓冲区插入关键词
uint32_t Ring_Fifo_Find_Keyword(ring_Fifo *ring_Fifo_handle, uint32_t keyword, uint8_t keyword_lenght);//从头指针开始查找最近的匹配字符
static uint32_t Ring_Fifo_Get_Word(ring_Fifo *ring_Fifo_handle, uint32_t head, uint32_t read_lenght);//从指定头指针地址获取完整长度的关键词（私有函数，无指针越位保护）
uint32_t Ring_Fifo_Get_Lenght(ring_Fifo *ring_Fifo_handle);//获取缓冲区里已储存的数据长度
uint32_t Ring_Fifo_Get_FreeSize(ring_Fifo *ring_Fifo_handle);//获取缓冲区可用储存空间


//---------------------------------GXT添加内容-BEGIN-----------------------
//CPP工程
#ifdef __cplusplus
    #define gRingFifo_En_Cpp 1    //是否支持cpp (cout << endl)
#endif

#if gRingFifo_En_Cpp//GXT-使用C++，方便使用
class gRingFifo
{
private:
    /* data */
    uint32_t num;
    uint8_t *ringFifoBuf;
    ring_Fifo *ringFifoHandle;
public:
    //初始化构造函数
    gRingFifo(uint32_t size){
        num=size;
        ringFifoBuf=new uint8_t [size];//在堆区创建需要的数组
        ringFifoHandle=new ring_Fifo;
        Ring_Fifo_Init(ringFifoHandle,ringFifoBuf,size);
    }
    //获取句柄
    ring_Fifo * getHandle(){
        return ringFifoHandle;
    }
    //获取存储buf首地址
    uint8_t * getBufAdd(){
        return ringFifoBuf;
    }
    //获取缓冲区长度
    uint32_t getWholeNum(){
        return num;
    }
    //获取缓冲区可用长度
    uint32_t getUnusedNum(){
        return Ring_Fifo_Get_FreeSize(ringFifoHandle);
    }
    //获取缓冲区里已储存的数据长度
    uint32_t getUsedNum(){
        return Ring_Fifo_Get_Lenght(ringFifoHandle);
    }
    //写入一个字节,返回写入成功或失败
    uint8_t push(uint8_t data){
        return Ring_Fifo_Write_Byte(ringFifoHandle,data);
    }
    //读取一个字节，返回读取的字节
    uint8_t pop(){
        return Ring_Fifo_Read_Byte(ringFifoHandle);
    }
    //写入一串字节,返回写入成功或失败
    uint8_t push(uint8_t *address,uint32_t num){
        return Ring_Fifo_Write_String(ringFifoHandle,address,num);
    }
    //写入一串字节,返回写入成功或失败,适应char*类型
    uint8_t push(char *address,uint32_t num){
        return Ring_Fifo_Write_String(ringFifoHandle,(uint8_t*)address,num);
    }
    //读取一串字节,参数为要存储的地址和读取个数，返回读取成功或失败
    uint8_t pop(uint8_t *address,uint32_t num){
        return Ring_Fifo_Read_String(ringFifoHandle,address,num);
    }
    //从头指针开始删除指定长度的数据，返回删除是否成功
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
//---------------------------------GXT添加内容-END-----------------------


#endif // !RINGFIFO_H__
