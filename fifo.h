#ifndef _FIFO_H
#define _FIFO_H
#include <stdlib.h>
#include <string.h>

#ifdef __GNUC__
#define smp_wmb __sync_synchronize
#elif 
#define smp_wmb MemoryBarrier()
#elif
#define smp_wmb 
#endif

typedef void * Handlefifo;

/**********************************************
 *函数名称:
 *功    能:初始化fifo
 *输入参数:
 *返 回 值:Handlefifo类型
 *说    明:
**********************************************/
Handlefifo fifo_init();

/**********************************************
 *函数名称:
 *功    能:分配fifo空间大小
 *输入参数:pfifo 已初始化的Handlefifo类型
           size fifo的容量大小
           esize fifo的元素大小
 *返 回 值: 0 成功
            1 失败
 *说    明:分配的总空间大小为size*esize size>0 esize>1
**********************************************/
unsigned int fifo_alloc(Handlefifo pfifo, unsigned int size, unsigned int esize);

/**********************************************
 *函数名称:
 *功    能:数据进入队列
 *输入参数:pfifo 已初始化的Handlefifo类型
           addr 进入队列的数据地址
           len 数据长度
 *返 回 值: =len 成功
           !=len 失败 返回fifo空闲空间大小
 *说    明:
**********************************************/
unsigned int fifo_in(Handlefifo pfifo, void * addr, unsigned int len);

/**********************************************
 *函数名称:
 *功    能:数据离开队列
 *输入参数:pfifo 已初始化的Handlefifo类型
           addr 离开队列数据存放地址
 *返 回 值: =len 成功
           !=len 失败 为存储在fifo中数据大小
 *说    明:
**********************************************/
unsigned int fifo_out(Handlefifo pfifo, void * addr, unsigned int len);

/**********************************************
 *函数名称:
 *功    能:fifo中空闲空间大小
 *输入参数:pfifo 已初始化的Handlefifo类型
 *返 回 值:fifo中空闲空间大小
 *说    明:
**********************************************/
unsigned int fifo_avail(Handlefifo pfifo);

/**********************************************
 *函数名称:
 *功    能:fifo中已经使用空间大小
 *输入参数:pfifo 已初始化的Handlefifo类型
 *返 回 值:fifo中已经使用空间大小
 *说    明:
**********************************************/
unsigned int fifo_len(Handlefifo pfifo);

/**********************************************
 *函数名称:
 *功    能:销毁fifo
 *输入参数:pfifo 已初始化的Handlefifo类型
 *返 回 值:
 *说    明:
**********************************************/
void fifo_free(Handlefifo pfifo);

#endif