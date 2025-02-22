#include <stdio.h>
#include "data_manager.h"
#include "share_queue.h"
#include "fifo.h"

void* data_manager(Queue *Sharedata){
    int temp = 0;
    char buffer[256];
    temp = atoi(dequeue(Sharedata));

    if (temp <= TEMP_AVERAGE){
        snprintf(buffer, sizeof(buffer), "Sensor is so hot!!!\n");
        writeFifo(buffer);
    } else  {
        snprintf(buffer, sizeof(buffer), "Sensor is so hot!!!\n");
        writeFifo(buffer);
    }
}