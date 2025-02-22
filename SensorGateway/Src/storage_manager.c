#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "storage_manager.h"
#include "fifo.h"
#include "share_queue.h"

void *storage_manager(Queue *ShareData){
    sqlite3 *db;
    int retry_count = 0;
    char buffer[256];
    // Open database
    while (sqlite3_open("sensor_data.db", &db) != SQLITE_OK) {
        snprintf(buffer, sizeof(buffer), "Error in connection \n");
        writeFifo(buffer);
        if (++retry_count >= MAX_RETRY) {
            snprintf(buffer, sizeof(buffer), "Error over 3 times. Stop connction\n");
            writeFifo(buffer);
            return NULL;
        }
        sleep(WAIT_TIME);
    }
    snprintf(buffer, sizeof(buffer), "connection successful \n");
    writeFifo(buffer);

}