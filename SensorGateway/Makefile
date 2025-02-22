CC = gcc
CFLAGS = -Wall -Wextra -pthread -I$(INC_DIR)
LDFLAGS = -pthread -lsqlite3

CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin
# Object files
OBJ_FILES = $(OBJ_DIR)/fifo.o $(OBJ_DIR)/connection_manager.o $(OBJ_DIR)/data_manager.o $(OBJ_DIR)/storage_manager.o $(OBJ_DIR)/share_queue.o $(OBJ_DIR)client.o
# Targets
SERVER = server
SENSOR = client

make_dir:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(SERVER): $(CUR_DIR)/main.o $(OBJ_FILES)
	$(CC) $^ -o $@ $(LDFLAGS)

$(SENSOR): $(CUR_DIR)/client.o $(OBJ_FILES)
	$(CC) $^ -o $@ $(LDFLAGS)

# Object files
create_obj:
	$(CC) $(CFLAGS) -c -fPIC $(SRC_DIR)/fifo.c -o $(OBJ_DIR)/fifo.o
	$(CC) $(CFLAGS) -c -fPIC $(SRC_DIR)/connection_manager.c -o $(OBJ_DIR)/connection_manager.o
	$(CC) $(CFLAGS) -c -fPIC $(SRC_DIR)/data_manager.c -o $(OBJ_DIR)/data_manager.o
	$(CC) $(CFLAGS) -c -fPIC $(SRC_DIR)/storage_manager.c -o $(OBJ_DIR)/storage_manager.o
	$(CC) $(CFLAGS) -c -fPIC $(CUR_DIR)/main.c -o $(CUR_DIR)/main.o
	$(CC) $(CFLAGS) -c -fPIC $(CUR_DIR)/client.c -o $(CUR_DIR)/client.o 

all: create_obj make_dir $(SERVER) $(SENSOR)  
clean:
	rm -f *.o $(SERVER) $(SENSOR) gateway.log logFifo sensor_data.db
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(BIN_DIR)/*
.PHONY: all clean make_dir create_obj