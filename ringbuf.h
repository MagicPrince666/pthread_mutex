#ifndef __RINGBUF_H_
#define __RINGBUF_H_

#include <stdlib.h>
#include <pthread.h>

typedef struct {
    char*  buf;
    unsigned int   size;
    unsigned int   in;
    unsigned int   out;
} cycle_buffer;

cycle_buffer* ring_init(int length);
int cycle_destroy(cycle_buffer* buffer);

int cycle_read(cycle_buffer* buffer, char *target, unsigned int amount);
int cycle_write(cycle_buffer* buffer, char *data, unsigned int length);
int cycle_empty(cycle_buffer* buffer);
int cycle_overage(cycle_buffer* buffer);
int cycle_reset(cycle_buffer* buffer);

#endif
