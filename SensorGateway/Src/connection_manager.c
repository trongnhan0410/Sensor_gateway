#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>      
#include <netinet/in.h>   
#include <arpa/inet.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include "fifo.h"
#include "connection_manager.h"
#include "share_queue.h"

#define PORT_NO 1234
#define MAX_CLIENTS 5

int server_fd;
struct sockaddr_in client_addr;
struct pollfd fds[MAX_CLIENTS + 1];  
int num_clients = 0;

    void socket_init(void)
    {
        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(struct sockaddr_in));
        memset(&client_addr, 0, sizeof(struct sockaddr_in));

        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1)
            handle_error("socket()");

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
            handle_error("setsockopt()");  

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port_no);
        serv_addr.sin_addr.s_addr =  INADDR_ANY; 

        if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
            handle_error("bind()");

        if (listen(server_fd, MAX_CLIENTS) == -1)
            handle_error("listen()");
    }

    void* handle_connection(Queue *ShareData){
        socket_init();
        int new_client_fd;
        int len = sizeof(struct sockaddr_in);
        memset(&client_addr, 0, sizeof(struct sockaddr_in));
        fds[0].fd = server_fd;
        fds[0].events = POLLIN;
        for (int i = 1; i <= MAX_CLIENTS; i++) {
            fds[i].fd = -1; 
        }
        while (1)
        {
            int activity = poll(fds, num_clients + 1, 1000);
            if (activity < 0) {
                perror("Poll error");
                break;
            }

            if ((fds[0].revents & POLLIN) && (activity > 0) && (num_clients <= MAX_CLIENTS)) {
                new_client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
                if (new_client_fd < 0) {
                    perror("Accept failed");
                }
                for (int i = 1; i <= MAX_CLIENTS; i++) {
                    if (fds[i].fd == -1) { 
                        fds[i].fd = client_fd;
                        fds[i].events = POLLIN;  
                        num_clients++;
                        break;
                    }
                }
            }
            for (int i = 1; i <= MAX_CLIENTS; i++) {
                if (fds[i].fd != -1 && (fds[i].revents & POLLIN)) {
                    char buffer[1024] = {0};
                    int valread = read(fds[i].fd, buffer, sizeof(buffer));
    
                    if (valread == 0) {
                        // close client socket
                        close(fds[i].fd);
                        fds[i].fd = -1;
                        num_clients--;
                    } else {
                        printf("Client_fd %d send data: %s\n", i, buffer);
                       // send data to sharequeue
                       enqueue(ShareData,buffer);
                    }
                }
            }
        }
        close(server_fd);
    }