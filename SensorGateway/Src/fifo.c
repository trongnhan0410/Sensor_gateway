#include "fifo.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t flock = PTHREAD_MUTEX_INITIALIZER;

void writeFifo(const char *data){
 
    pthread_mutex_lock(&flock);
    int fd = open(FIFO_FILE, O_WRONLY);
    if (fd != -1)
    {
        ssize_t bytes_written = write(fd, data, strlen(data));
        if (bytes_written < 0)
        {
            perror("write to FIFO failed");
        }
        close(fd);
    }
    pthread_mutex_unlock(&flock);
}

void log_process(){
    int fd_log = open("gateway.log", O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd_log == -1) {
        perror("Error opening gateway.log");
        return;
    }

    int fd = open(FIFO_FILE, O_RDONLY);
    if (fd == -1)
    {
        perror("Failed to open FIFO");
        exit(1);
    }

    int sequence = 0;
    char timestamp[20];
    char log_entry[256];

    while (1)
    {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
        ssize_t bytes_read = read(fd, log_entry, sizeof(log_entry) - 1);
        if (bytes_read < 0)
        {
            perror("read from FIFO failed");
            break;
        }
        log_entry[bytes_read] = '\0';
        dprintf(fd_log, "%s %d %s", timestamp, sequence++, log_entry);
        close(fd_log);
    }
}