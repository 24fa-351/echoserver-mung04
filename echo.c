#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9000

int main(int argc, char* argv[]) {
    
    int fd_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (fd_socket == -1) {
        printf("Failed creation\n");
        exit(0);
    }

    // Segmentation fault w/o "-p"?? Also doesn't run printf
    if (strcmp(argv[1], "-p")) {
        printf("Current Port: %d\n", PORT);
    }

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);    

    int bindval = bind(fd_socket, (struct sockaddr*)&server_address, sizeof(server_address));

    if (bindval < 0) {
        printf("Bind failed\n");
        exit(0);
    }

    bindval = listen(fd_socket, 5);

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int conn_sock_fd = accept(fd_socket, (struct sockaddr*)&client_addr, &addr_len);

    char buffer[1024];
    while (1) {
        int bytes = read(conn_sock_fd, buffer, sizeof(buffer));
        printf("Received: %s\n", buffer);
        write(conn_sock_fd, buffer, bytes);
        if (bytes < 0) {
            printf("Exiting...\n");
            break;
        }
    }

    close(conn_sock_fd);

    return 0;
}