#include <stdio.h>
#include <pthread.h>
#include "fifo.h"

Handlefifo fifo;

void * in(void * arg){
    for(int i=0; i<10000000; i++){
        while(1){
            if(fifo_avail(fifo) > 0)
            break;
        }

        int a = i + 1;
        fifo_in(fifo, &a, sizeof(int));
    }
}

void * out(void * arg){
    while(1){
        if(fifo_len(fifo) > 0){
            int b;
            fifo_out(fifo, &b, sizeof(int));

            printf("%d\n", b);

        }
    }

}

int main()
{
    
    fifo = fifo_init();
    fifo_alloc(fifo, 1024, sizeof(int));

    pthread_t in_t, out_t;
    
    pthread_create(&in_t, NULL, in, NULL);
    pthread_create(&out_t ,NULL, out, NULL);

    pthread_join(in_t, NULL);
    pthread_join(out_t, NULL);

    

}