#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h> // for function exit()

/*
 * struct sockaddr_in{
 * short            sin_family; // socket family e.g. AF_INET, AF_INET6
 * unsigned short   sin_port; // port number htons(3940) -- host to network (big endian)
 * struct in_addr   sin_addr; 
 * char             sin_zero[8]; // what is this for?? Seems like a bullshit to bamboozle people
 * }
 * inet_pton: convert presentation to network: from dotted notation 127.0.0.1 to hex 0x87FC
 * inet_ntop: convert network to presentation
 * struct in_addr{
 * unsigned long    s_addr;    // load with inet_pton()
 * }
 */
void connect_to_server(int socket_desc, char* ip_addr, int port_num){
    // creating the server struct
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port_num); // convert to big endian
    server.sin_addr.s_addr = inet_addr(ip_addr);
    
    // use function connect to first establish connection with server
    // connect(int socket, sockaddr *addr, socklen_t address_len)
    // address_len: passed in sizeof(struct sockaddr)
    // :return 0 for success, -1 for failure and errno FLAG set
    printf("connecting socket %d to (%s : %d)\n", socket_desc, ip_addr, port_num);

    if (connect(socket_desc, &server, sizeof(server)) == -1){
        perror("connect unsuccessful");
        exit(-1);
    }
    printf("connected\n");
    return;
}
int main()
{
    
    int socket_desc;
    // high level: socket is a pair of (ip_address, port_number)
    // port_number is a software logic construct to bind to a specific process and establish
    // convention. For example, port 80 is typically used for web server.
    // ----- C programming --------
    // socket(domain, type, protocol)
    // return: file descriptor of the socket.
    // AF_INET is the IPv4 
    // sock_stream is the reliable 2-way communication. TCP protocol
    // 0 protocol: normally only 1 protocol exists to support any given type like SOCK_STREAM

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    printf("Created socket_desc at %d\n", socket_desc);
    

    char* ip_addr = "128.119.243.147";
    int port_num = 27993;

    connect_to_server(socket_desc, ip_addr, port_num);
    return 0;
}

