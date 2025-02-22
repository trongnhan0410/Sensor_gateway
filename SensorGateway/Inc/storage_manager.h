#ifndef _STORAGE_MANAGER_H_
#define _STORAGE_MANAGER_H_

#define MAX_RETRY 3
#define WAIT_TIME 1

int insert_data(SensorData *data);
void *storage_manager(Queue *ShareData);

#endif