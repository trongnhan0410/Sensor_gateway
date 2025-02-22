#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include "connection_manager.h"
#include "share_queue.h"
#include "storage_manager.h"

#define MAX_QUEUE   12

Queue share_data;

pthread_t connection_thread, data_storage_thread, data_manager_thread;

void run_log_process(){
    log_process();
}

void run_main_process(){
    pthread_create(&connection_thread, NULL, handle_connection(&share_data), NULL);
    pthread_create(&data_storage_thread, NULL, storage_manager(&share_data), NULL);
    pthread_create(&data_manager_thread, NULL, data_manager(&share_data), NULL);

    pthread_join(data_manager_thread, NULL);
    pthread_join(data_storage_thread, NULL);
    pthread_join(connection_thread, NULL);
}

int main(){
    
    initialize(&share_data, MAX_QUEUE);

    if (mkfifo(FIFO_NAME, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("mkfifo");
            exit(1);
        }
    }
    pid_t pid = fork();
    if (pid == 0) { // Log process
        run_log_process();
        exit(0);
    } else if (pid > 0) { // Main process
       run_main_process();
    } else {
        perror("Fork failed");
        exit(1);
    }

    return 0;
}