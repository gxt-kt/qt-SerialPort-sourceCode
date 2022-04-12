#include "ringfifo.h"


/*GXT_KT:EXAMPLE-C++
#define Read_Fifo_SIZE	256
#include "ringfifo.h"
use namespace std;
int main()
{
    //��ʼ������
    gRingFifo cdcFifo(Read_Fifo_SIZE);
    
    cdcFifo.push("hello world",11);
    
    uint32_t length=cdcFifo.getUsedNum();
    if(length){
        uint8_t readBuf[Read_Fifo_SIZE];
        cdcFifo.pop(readBuf,length);
        cout << "test: " << readBuf<< endl;
    }
}
*/

/*ԭ����EXAMPLE-C
#define Read_Fifo_SIZE	256
int main()
{
    //�½�������������RingFifo�������
    uint8_t Fifo[Read_Fifo_SIZE] ;
    ring_Fifo RB ;
    
    //��ʼ��RingFifo����������󶨻��������飻
    Ring_Fifo_Init(&RB, Fifo, Read_Fifo_SIZE);
    
    //���λ�����д��һ���ֽں�һ���ֽ�
    Ring_Fifo_Write_String(&RB, "hello world", 11);
    Ring_Fifo_Write_Byte(&RB, '!');
    
    //��ȡ�Ѵ�������ݳ��ȣ��������λ������е����ݲ���ӡ
    uint32_t num = Ring_Fifo_Get_Lenght(&RB);
    uint8_t get[16] ;
    Ring_Fifo_Read_String(&RB, get, num);
    printf("%s", get);
}
*/


/**
 * \file ring_Fifo.c
 * \brief ���׻��λ����ʵ��
 * \details ����Ϊ��Ƭ�����ڵĻ��λ���/��FIFO�������������ݴ���������
 * �������λ��������ٵĴ���/��ȡ���ݣ����������ֶ���¼���ݽ���������׼ȷ�Ķ����������ݣ�
 * �����ֶ�������ݻ�������ֻҪ���ϴν��յ����ݶ�ȡ����������������׼���ý�����һ�����ݣ�
 * ��ʡ���ֶ������ͨ��������ʱ�䣬�ܹ��������ڳ��������Ч�ʣ�
 * \author netube_99\netube@163.com
 * \date 2021.06.29
 * \version v1.3.2
 * 
 * 2021.01.19 v1.0.0 ������һ�汾
 * 2021.01.24 v1.1.0 ����ƥ���ַ����Һ���
 * 2021.01.27 v1.2.0 ����ƥ���ַ����ҹ��ܣ�����֧��8λ��32λ�ؼ��ʲ�ѯ
 * 2021.01.28 v1.3.0 ��λ�����޸�Ϊɾ�����������ӹؼ��ʲ��뺯��������Ӧ��С�ˣ�
 * 2021.01.30 v1.3.1 �޸���String��д������С����ָ���������
 * 2021.06.29 v1.3.2 �޸��� Ring_Fifo_Write_String �Ĳ������ʹ���
                    �޸��� Ring_Fifo_Write_Byte �޷�д�������һλ������
 * 2022.02.17 v2.0.0 ������delete�����Ĳ������ͣ�ԭ����uint8_t,����Ϊuint32_t
                    �����C++�࣬��cԴ��������Ϸ�װ��Ӧ���ܣ�����ȫ����.h�ļ����·�
*/


/**
 * \brief ��ʼ���»�����
 * \param[out] ring_Fifo_handle: ����ʼ���Ļ������ṹ����
 * \param[in] Fifo_addr: �ⲿ����Ļ��������飬���ͱ���Ϊ uint8_t
 * \param[in] Fifo_size: �ⲿ����Ļ���������ռ�
 * \return ���ػ�������ʼ���Ľ��
 *      \arg RING_FIFO_SUCCESS: ��ʼ���ɹ�
 *      \arg RING_FIFO_ERROR: ��ʼ��ʧ��
*/
uint8_t Ring_Fifo_Init(ring_Fifo *ring_Fifo_handle, uint8_t *Fifo_addr ,uint32_t Fifo_size)
{
    ring_Fifo_handle->head = 0 ;//��λͷָ��
    ring_Fifo_handle->tail = 0 ;//��λβָ��
    ring_Fifo_handle->lenght = 0 ;//��λ�Ѵ洢���ݳ���
    ring_Fifo_handle->array_addr = Fifo_addr ;//�����������������ַ
    ring_Fifo_handle->max_lenght = Fifo_size ;//���������ɴ���������
    if(ring_Fifo_handle->max_lenght < 2)//�������������������Ԫ������
        return RING_FIFO_ERROR ;//�����������С�����г�ʼ��ʧ��
    else
        return RING_FIFO_SUCCESS ;//��������ʼ���ɹ�
}

/**
 * \brief ��ͷָ�뿪ʼɾ��ָ�����ȵ�����
 * \param[out] ring_Fifo_handle: �������ṹ����
 * \param[in] lenght: Ҫɾ���ĳ���
 * \return ����ɾ��ָ���������ݽ��
 *      \arg RING_FIFO_SUCCESS: ɾ���ɹ�
 *      \arg RING_FIFO_ERROR: ɾ��ʧ��
*/
uint8_t Ring_Fifo_Delete(ring_Fifo *ring_Fifo_handle, uint32_t lenght)
{
    if(ring_Fifo_handle->lenght < lenght)
        return RING_FIFO_ERROR ;//�Ѵ����������С����ɾ����������
    else
    {
        if((ring_Fifo_handle->head + lenght) >= ring_Fifo_handle->max_lenght)
            ring_Fifo_handle->head = lenght - (ring_Fifo_handle->max_lenght - ring_Fifo_handle->head);
        else
            ring_Fifo_handle->head += lenght ;//ͷָ����ǰ�ƽ�����������
        ring_Fifo_handle->lenght -= lenght ;//���¼�¼��Ч���ݳ���
        return RING_FIFO_SUCCESS ;//�Ѵ����������С����ɾ����������
    }
}

/**
 * \brief �򻺳���β��дһ���ֽ�
 * \param[out] ring_Fifo_handle: �������ṹ����
 * \param[in] data: Ҫд����ֽ�
 * \return ���ػ�����д�ֽڵĽ��
 *      \arg RING_FIFO_SUCCESS: д��ɹ�
 *      \arg RING_FIFO_ERROR: д��ʧ��
*/
uint8_t Ring_Fifo_Write_Byte(ring_Fifo *ring_Fifo_handle, uint8_t data)
{
    //�����������������������Ǵ���
    if(ring_Fifo_handle->lenght == (ring_Fifo_handle->max_lenght))
        return RING_FIFO_ERROR ;
    else
    {
        *(ring_Fifo_handle->array_addr + ring_Fifo_handle->tail) = data;//����ַ+ƫ�������������
        ring_Fifo_handle->lenght ++ ;//����������+1
        ring_Fifo_handle->tail ++ ;//βָ�����
    }
    //���βָ�볬Խ������ĩβ��βָ��ָ�򻺳������鿪ͷ���γɱջ�
    if(ring_Fifo_handle->tail > (ring_Fifo_handle->max_lenght - 1))
        ring_Fifo_handle->tail = 0 ;
	return RING_FIFO_SUCCESS ;
}

/**
 * \brief �ӻ�����ͷָ���ȡһ���ֽ�
 * \param[in] ring_Fifo_handle: �������ṹ����
 * \return ���ض�ȡ���ֽ�
*/
uint8_t Ring_Fifo_Read_Byte(ring_Fifo *ring_Fifo_handle)
{
    uint8_t data ;
    if (ring_Fifo_handle->lenght != 0)//������δ����
    {
        data = *(ring_Fifo_handle->array_addr + ring_Fifo_handle->head);//��ȡ����
        ring_Fifo_handle->head ++ ;
        ring_Fifo_handle->lenght -- ;//����������-1
        //���ͷָ�볬Խ������ĩβ��ͷָ��ָ�����鿪ͷ���γɱջ�
        if(ring_Fifo_handle->head > (ring_Fifo_handle->max_lenght - 1))
            ring_Fifo_handle->head = 0 ;
    }
    return data ;
}

/**
 * \brief �򻺳���β��дָ�����ȵ�����
 * \param[out] ring_Fifo_handle: �������ṹ����
 * \param[out] input_addr: ��д�����ݵĻ���ַ
 * \param[in] write_lenght: Ҫд����ֽ���
 * \return ���ػ�����β��дָ�������ֽڵĽ��
 *      \arg RING_FIFO_SUCCESS: д��ɹ�
 *      \arg RING_FIFO_ERROR: д��ʧ��
*/
uint8_t Ring_Fifo_Write_String(ring_Fifo *ring_Fifo_handle, uint8_t *input_addr, uint32_t write_lenght)
{
    //��������洢�ռ���������,���ش���
    if((ring_Fifo_handle->lenght + write_lenght) > (ring_Fifo_handle->max_lenght))
        return RING_FIFO_ERROR ;
    else
    {
        //��������д�볤��
        uint32_t write_size_a, write_size_b ;
        //���˳����ó���С����д��ĳ��ȣ���Ҫ�����ݲ�����ηֱ�д��
        if((ring_Fifo_handle->max_lenght - ring_Fifo_handle->tail) < write_lenght)
        {
            write_size_a = ring_Fifo_handle->max_lenght - ring_Fifo_handle->tail ;//��βָ�뿪ʼд����������ĩβ
            write_size_b = write_lenght - write_size_a ;//�Ӵ������鿪ͷд����
        }
        else//���˳����ó��ȴ��ڻ������д��ĳ��ȣ���ֻ��Ҫд��һ��
        {
            write_size_a = write_lenght ;//��βָ�뿪ʼд����������ĩβ
            write_size_b = 0 ;//����Ӵ������鿪ͷд����
        }
        //��ʼд������
        if(write_size_b != 0)//��Ҫд������
        {
            //�ֱ𿽱�a��b�����ݵ�����������
            memcpy(ring_Fifo_handle->array_addr + ring_Fifo_handle->tail, input_addr, write_size_a);
            memcpy(ring_Fifo_handle->array_addr, input_addr + write_size_a, write_size_b);
            ring_Fifo_handle->lenght += write_lenght ;//��¼�´洢�˶���������
            ring_Fifo_handle->tail = write_size_b ;//���¶�λβָ��λ��
        }
        else//ֻ��д��һ��
        {
            memcpy(ring_Fifo_handle->array_addr + ring_Fifo_handle->tail, input_addr, write_size_a);
            ring_Fifo_handle->lenght += write_lenght ;//��¼�´洢�˶���������
            ring_Fifo_handle->tail += write_size_a ;//���¶�λβָ��λ��
            if(ring_Fifo_handle->tail == ring_Fifo_handle->max_lenght)
                ring_Fifo_handle->tail = 0 ;//���д�����ݺ�βָ��պ�д������β������ص���ͷ����ֹԽλ
        }
        return RING_FIFO_SUCCESS ;
    }
}

/**
 * \brief �򻺳���ͷ����ָ�����ȵ����ݣ����浽ָ���ĵ�ַ
 * \param[in] ring_Fifo_handle: �������ṹ����
 * \param[out] output_addr: ��ȡ�����ݱ����ַ
 * \param[in] read_lenght: Ҫ��ȡ���ֽ���
 * \return ���ػ�����ͷ����ָ�������ֽڵĽ��
 *      \arg RING_FIFO_SUCCESS: ��ȡ�ɹ�
 *      \arg RING_FIFO_ERROR: ��ȡʧ��
*/
uint8_t Ring_Fifo_Read_String(ring_Fifo *ring_Fifo_handle, uint8_t *output_addr, uint32_t read_lenght)
{
    if(read_lenght > ring_Fifo_handle->lenght)
        return RING_FIFO_ERROR ;
    else
    {
        uint32_t Read_size_a, Read_size_b ;
        if(read_lenght > (ring_Fifo_handle->max_lenght - ring_Fifo_handle->head))
        {
            Read_size_a = ring_Fifo_handle->max_lenght - ring_Fifo_handle->head ;
            Read_size_b = read_lenght - Read_size_a ;
        }
        else
        {
            Read_size_a = read_lenght ;
            Read_size_b = 0 ;
        }
        if(Read_size_b != 0)//��Ҫ��ȡ����
        {
            memcpy(output_addr, ring_Fifo_handle->array_addr + ring_Fifo_handle->head, Read_size_a);
            memcpy(output_addr + Read_size_a, ring_Fifo_handle->array_addr, Read_size_b);
            ring_Fifo_handle->lenght -= read_lenght ;//��¼ʣ��������
            ring_Fifo_handle->head = Read_size_b ;//���¶�λͷָ��λ��
        }
        else
        {
            memcpy(output_addr, ring_Fifo_handle->array_addr + ring_Fifo_handle->head, Read_size_a);
            ring_Fifo_handle->lenght -= read_lenght ;//��¼ʣ��������
            ring_Fifo_handle->head += Read_size_a ;//���¶�λͷָ��λ��
            if(ring_Fifo_handle->head == ring_Fifo_handle->max_lenght)
                ring_Fifo_handle->head = 0 ;//�����ȡ���ݺ�ͷָ��պ�д������β������ص���ͷ����ֹԽλ
        }
        return RING_FIFO_SUCCESS ;
    }
}

/**
 * \brief ���λ���������ؼ���
 * \param[in] ring_Fifo_handle: �������ṹ����
 * \param[in] keyword: Ҫ����Ĺؼ���
 * \param[in] keyword_lenght:�ؼ����ֽ��������4�ֽڣ�32λ��
 * \return ���ز���ؼ��ʵĽ��
 *      \arg RING_FIFO_SUCCESS: ����ɹ�
 *      \arg RING_FIFO_ERROR: ����ʧ��
*/
uint8_t Ring_Fifo_Insert_Keyword(ring_Fifo *ring_Fifo_handle, uint32_t keyword, uint8_t keyword_lenght)
{
    uint8_t *keyword_addr = (uint8_t *)&keyword;
    uint8_t keyword_byte[4];
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    //С��ģʽ�ֽ�������
    keyword_byte[0] = *(keyword_addr + 3) ;
    keyword_byte[1] = *(keyword_addr + 2) ;
    keyword_byte[2] = *(keyword_addr + 1) ;
    keyword_byte[3] = *(keyword_addr + 0) ;
    //���ؼ������뻷�λ�����
    return Ring_Fifo_Write_String(ring_Fifo_handle, keyword_byte, keyword_lenght);
#else
    //���ģʽ�ֽ�������
    keyword_byte[0] = *(keyword_addr + 0) ;
    keyword_byte[1] = *(keyword_addr + 1) ;
    keyword_byte[2] = *(keyword_addr + 2) ;
    keyword_byte[3] = *(keyword_addr + 3) ;
    //���ؼ������뻷�λ�����
    return Ring_Fifo_Write_String(ring_Fifo_handle, keyword_byte + (keyword_lenght - 1), keyword_lenght);
#endif
}

/**
 * \brief ��ͷָ�뿪ʼ���������ƥ��ؼ���
 * \param[in] ring_Fifo_handle: �������ṹ����
 * \param[in] keyword: Ҫ���ҵĹؼ���
 * \param[in] keyword_lenght:�ؼ����ֽ��������4�ֽڣ�32λ��
 * \return ���ػ�ȡƥ���ַ����λ��Ҫ��ȡ���ֽ��������� 0/RING_FIFO_ERROR: �����ʧ��
*/
uint32_t Ring_Fifo_Find_Keyword(ring_Fifo *ring_Fifo_handle, uint32_t keyword, uint8_t keyword_lenght)
{
    uint32_t max_find_lenght = ring_Fifo_handle->lenght - keyword_lenght + 1 ;//������Ҫ��������󳤶�
    uint8_t trigger_word = keyword >> ((keyword_lenght - 1) * 8) ;//���㴥���ؼ��ʼ����ֽڣ����λ��
    uint32_t distance = 1 , find_head = ring_Fifo_handle->head;//��¼�ؼ��ʾ���ͷָ��ĳ���/��ʱͷָ���ȡԭͷָ���ʼֵ
    while(distance <= max_find_lenght)//���趨��Χ�������ؼ��ʣ���ָֹ��Խλ����
    {
        if(*(ring_Fifo_handle->array_addr + find_head) == trigger_word)//�����λ�ֽ�ƥ����ʼ���λ���
            if(Ring_Fifo_Get_Word(ring_Fifo_handle, find_head, keyword_lenght) == keyword)//����ؼ���ƥ��
                return distance ;//���س��ȣ�ʹ�� Ring_Fifo_Read_String ����ȡ����
        find_head ++ ;//��ǰ�ַ���ƥ�䣬��ʱͷָ����ƣ������һ��
        distance ++ ;//����ҲҪ�����������Ⱥ���
        if(find_head > (ring_Fifo_handle->max_lenght - 1))
            find_head = 0 ;//�����������β�����򷵻����鿪ͷ�����λ�������ԣ�
    }
    return RING_FIFO_ERROR ;//�㶼ĳ����
}

/**
 * \brief ��ָ��ͷָ���ַ��ȡ�������ȵĹؼ��ʣ�˽�к�������ָ��Խλ������
 * \param[in] ring_Fifo_handle: �������ṹ����
 * \param[in] head: ͷָ��ƫ������ƥ���ַ����ڵ�ַ��
 * \param[in] read_lenght:�ؼ����ֽ��������4�ֽڣ�32λ��
 * \return ���������Ĺؼ���
*/
static uint32_t Ring_Fifo_Get_Word(ring_Fifo *ring_Fifo_handle, uint32_t head, uint32_t read_lenght)
{
    uint32_t data = 0, i ;
    for(i=1; i<=read_lenght; i++)//���չؼ��ʵĳ��ȣ��ַ����������ȡ����
    {
        //�����λ�����λ�����ϳ�һ��32λ����
        data |= *(ring_Fifo_handle->array_addr + head) << (8*(read_lenght - i)) ;
        head ++ ;
        if(head > (ring_Fifo_handle->max_lenght - 1))
            head = 0 ;//�����������β�����򷵻����鿪ͷ�����λ�������ԣ�
    }
    return data ;//������λ���Ϻ��32λ����
}

/**
 * \brief ��ȡ���������Ѵ�������ݳ���
 * \param[in] ring_Fifo_handle: �������ṹ����
 * \return ���ػ��������Ѵ�������ݳ���
*/
uint32_t Ring_Fifo_Get_Lenght(ring_Fifo *ring_Fifo_handle)
{
    return ring_Fifo_handle->lenght ;
}

/**
 * \brief ��ȡ���������ô���ռ�
 * \param[in] ring_Fifo_handle: �������ṹ����
 * \return ���ػ��������ô���ռ�
*/
uint32_t Ring_Fifo_Get_FreeSize(ring_Fifo *ring_Fifo_handle)
{
    return (ring_Fifo_handle->max_lenght - ring_Fifo_handle->lenght) ;
}