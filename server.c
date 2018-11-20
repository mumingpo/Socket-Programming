#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h> // for close
#include <arpa/inet.h>

int main(){
    printf("Starting server\n");
    char server_msg[256] = "Reached the server\n";

    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9002);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // generate a random IP
    

    // bind the socket to specific IP and port so that client can find
    // the server
    bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    
    // listen(int socket, int backlog)
    // all incoming connections are queued. backlog is the maximum capacity
    // of queue of pending connections
    listen(server_socket,2 );
    
    // accept(socket, sockaadr, addr_len)
    // socket: the server_socket
    // sockaddr: address of the connecting client socket would be copied to this
    // addr_len: the length of the connecting client socket
    // we're not storing the client

    int client_socket = accept(server_socket, NULL, NULL);
    

    char client_msg[1000];
    recv(client_socket, &client_msg, sizeof(client_msg), 0);

    printf("Client message: %s \n", client_msg);

    printf("Server sending message: %s \n", server_msg);

    send(client_socket, server_msg, sizeof(server_msg),0);
    
    close(server_socket);
    printf("Shutting down server\n");
    return 0;
}
