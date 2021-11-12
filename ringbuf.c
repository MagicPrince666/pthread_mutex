#include "ringbuf.h"
#include <string.h>
#include <stdio.h>

#define Min(x, y) ((x) < (y) ? (x) : (y))

//extern pthread_mutex_t mut;//声明互斥变量

int buf_size = 0;

cycle_buffer* ring_init(int length)
{
    printf("Init cycle buffer\n");

    cycle_buffer* buffer = (cycle_buffer *)malloc(sizeof(cycle_buffer));
    if (!buffer) {
        printf("malloc cycle buffer error\n");
        return NULL;
    }
    memset(buffer, 0, sizeof(cycle_buffer)); 

    buffer->size = length;  
    buffer->in   = 0;
    buffer->out  = 0;  

    buffer->buf = (char *)malloc(buffer->size);  
    if (!buffer->buf){
        free(buffer);
        printf("malloc cycle buffer size error\n");
        return NULL;
    }
    memset(buffer->buf, 0, length);

    //pthread_mutex_init(&mut,NULL);
    buf_size = length;
    return buffer;
}

int cycle_destroy(cycle_buffer* buffer)
{
    if (!buffer->buf) {
       free(buffer->buf);
       buffer->buf = NULL;
       printf("free buffer->buf\n");
    }
    if (!buffer) {
       free(buffer);
       buffer = NULL;
       printf("free buffer\n");
    }
    printf("Clean cycle buffer\n");
    return 0;
}

int cycle_reset(cycle_buffer* buffer)
{
    if (buffer == NULL) {
        return -1;
    }

    buffer->in   = 0;
    buffer->out  = 0;
    memset(buffer->buf, 0, buffer->size);

    printf("RingBuffer cleaned\n");

    return 0;
}

int cycle_empty(cycle_buffer* buffer)
{
    return buffer->in == buffer->out;
}

//get buffer size canbe use
int cycle_overage(cycle_buffer* buffer) {
    int overage = buffer->in - buffer->out;

    if(overage > 0)
        return buf_size - overage;
    else
        return buf_size + overage;
}

int cycle_write(cycle_buffer* buffer, char *data, unsigned int length)
{
    unsigned int len = 0;

    length = Min(length, buffer->size - buffer->in + buffer->out);
    len    = Min(length, buffer->size - (buffer->in & (buffer->size - 1)));

    memcpy(buffer->buf + (buffer->in & (buffer->size - 1)), data, len);
    memcpy(buffer->buf, data + len, length - len);
 
    buffer->in += length;

    return length;
}

int cycle_read(cycle_buffer* buffer, char *target, unsigned int amount)
{
    unsigned int len = 0;

    amount = Min(amount, buffer->in - buffer->out);
    len    = Min(amount, buffer->size - (buffer->out & (buffer->size - 1)));
 
    //pthread_mutex_lock(&mut);
    memcpy(target, buffer->buf + (buffer->out & (buffer->size - 1)), len);
    memcpy(target + len, buffer->buf, amount - len);
    //pthread_mutex_unlock(&mut);
 
    buffer->out += amount;
 
    return amount;
}
