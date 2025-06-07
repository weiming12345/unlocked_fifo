#include "fifo.h"
#define min(x, y) ((x)<(y)?(x):(y))

struct fifo{
    unsigned int in;    //入口指针
    unsigned int out;   //出口指针
    unsigned int size;  //fifo大小
    unsigned int esize; //元素大小
    unsigned int mask;  
    void * data;        //数据指针
};

Handlefifo fifo_init() {

    Handlefifo pfifo;
    pfifo = (Handlefifo)malloc(sizeof(struct fifo));

    return pfifo;
}

/**********************************************
 *函数名称:
 *功    能:寻找大于等于一个数字的最小2的幂
 *输入参数:int
 *返 回 值:int
 *说    明:
**********************************************/
static inline unsigned int get_min_bin(unsigned int num) {
    num --;
    num |= num >> 1;
    num |= num >> 2;
    num |= num >> 4;
    num |= num >> 8;
    num |= num >> 16;
    num ++;
    return num;
}

unsigned int fifo_alloc(Handlefifo pfifo, unsigned int size, unsigned int esize) {

    struct fifo * fifo = (struct fifo *)pfifo;
    fifo->in           = 0;
    fifo->out          = 0;
    fifo->esize        = esize;
    fifo->size         = get_min_bin(size);
    fifo->mask         = fifo->size - 1;
    fifo->data         = (void *)malloc(fifo->size * fifo->esize);


    return 0;
}

static inline unsigned int fifo_unused(struct fifo * pfifo) {

    return pfifo->size - (pfifo->in - pfifo->out);

}

static inline volatile void fifo_copy_in(struct fifo * fifo, void * addr, unsigned len) {

    unsigned int size  = fifo->size;
    unsigned int mask  = fifo->mask;
    unsigned int esize = fifo->esize;
    unsigned int off   = fifo->in;
    unsigned int l;

    off &= mask;
    if(esize > 1) {
        off  *= esize;
        size *= esize;
        len  *= esize;
    }

    l = min(len, size - off);
    memcpy((char *)fifo->data + off, addr, l);
	memcpy(fifo->data, (char *)addr + l, len - l);

    smp_wmb();
    
}

unsigned int fifo_in(Handlefifo pfifo, void * addr, unsigned int len) {
    
    struct fifo * fifo = (struct fifo *)pfifo;
    unsigned int l     = fifo_unused(fifo);
    if(len > l)
        return l;
    fifo_copy_in(fifo, addr, len);
    fifo->in += len;

    return len;
}


static inline volatile void fifo_copy_out(struct fifo * fifo, void * addr, unsigned len) {

   unsigned int size  = fifo->size;
    unsigned int mask  = fifo->mask;
    unsigned int esize = fifo->esize;
    unsigned int off   = fifo->out;
    unsigned int l;

    off &= mask;
    if(esize > 1) {
        off  *= esize;
        size *= esize;
        len  *= esize;
    }

    l = min(len, size - off);

	memcpy(addr, (char *)fifo->data + off, l);
	memcpy((char *)addr + l, fifo->data, len - l);

    smp_wmb();

}

unsigned int fifo_out(Handlefifo pfifo, void * addr, unsigned int len) {

    struct fifo * fifo = (struct fifo *)pfifo;
    unsigned int l     = fifo->in - fifo->out;
    if(len > l)
        return l;
    fifo_copy_out(fifo, addr, len);
    fifo->out += len;

    return len;
}

unsigned int fifo_avail(Handlefifo pfifo) {

    struct fifo * fifo = (struct fifo *)pfifo;
    return ((fifo->size)-(fifo->in - fifo->out));

}

unsigned int fifo_len(Handlefifo pfifo) {

    struct fifo * fifo = (struct fifo *)pfifo;
    return (fifo->in - fifo->out);

}

void fifo_free(Handlefifo pfifo) {

    struct fifo * fifo = (struct fifo *)pfifo;
    if(fifo == NULL)
        return;
    if(fifo->data == NULL){
        free(fifo);
        return;
    }
    free(fifo->data);
    free(fifo);
    return;

}

