#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h> // for function exit()
#include <unistd.h> // for function read()
#include <string.h>


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
    
    printf("Starting client\n");
    printf("connecting socket %d to (%s : %d)\n", socket_desc, ip_addr, port_num);

    if (connect(socket_desc, &server, sizeof(server)) == -1){
        perror("connect unsuccessful");
        exit(-1);
    }
    printf("connected\n");
    return;
}

void send_msg(int socket, char* msg){
    //char msg[28] = "cs230 HELLO lnle@umass.net\n";

    // sending message to the server. 
    // send(int socket, char* msg, int sizeof(msg), int flags)
    // we can just ignore flags and set it to 0 for now.
    // :return: number of bytes sent or -1 for failure    
    printf("sending msg: %s \n", msg);
    printf("Not supposed to die??\n");
    int status = send(socket, msg, strlen(msg), 0);
    if (status == -1){
        perror("Sending msg failed");
        exit(-1);
    }
    printf("sending msg successfully. Number of bytes sent %d \n", status);
    return;
}

int solve_math(char* buffer){
   // logic to parse the buffer
   // strtok(char* string, char* separator)
   // splitting the string into words based on separator
   // :return the pointer to the beginning of the token
   // note that strtok modifies the original string which might cause segfault
   // ==> use strdup to create a duplicate of buffer
   char* buffer_dup = strdup(buffer);
   int first_operand;
    int second_operand;
    char operator;
    char* token = strtok(buffer_dup, " "); // split based on spaces

    int num_tokens = 0;
    int capture_flag = 0;
      while (token != NULL){
          printf("Copying %s\n", token);
            if (num_tokens == 2){
                printf("num_tokens 2 is %s\n", token);
                if (strcmp(token, "BYE\n") == 0){
                    printf("Capture the flag. Hail\n");
                    capture_flag = 1;
                    break;
                }
                first_operand = atoi(token);
            }
            if (num_tokens == 3){
                operator = token[0];
            }
            if (num_tokens == 4){
                second_operand = atoi(token);
            }

            token = strtok(NULL, " "); // NULL means parsing from the same string
            num_tokens ++;
      }
    
      if (capture_flag == 1){
          printf("Successfully capture the flag. Exiting \n");
          exit(0);
      }
    int result;
    printf("first_operand %d \n", first_operand);
    printf("second_operand %d \n", second_operand);
    printf("operator %c \n", operator);

        switch (operator) {
    case '+':
        result = first_operand + second_operand;
        break;
    case '-':
    result = first_operand - second_operand;
            break;
    case '*':
    result = first_operand * second_operand;
            break;

    case '/':
    result = first_operand / second_operand;
            break;

    default:
        printf("Default for operator %c\n", operator);
        result = -69999;
    }

printf("finished with result %d \n", result);
    return result;
}

int main(int argc, char* argv[])
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
    
    char* ip_addr;
    int port_num;
    if (argc == 1){
    ip_addr = "128.119.243.147";
    port_num = 27993;
    }
    else{
        ip_addr = argv[2];
        printf("argv[1]: %s\n", argv[1]);
        port_num = atoi(argv[1]); // string to integer
    }



    printf("port num %d\n", port_num);    
    // --- CONNECTING TO SERVER ------------
    connect_to_server(socket_desc, ip_addr, port_num);
    

    //  ---- SENDING FIRST MESSAGE -------
    char* msg = "cs230 HELLO lnle@umass.edu\n";
    send_msg(socket_desc, msg);
    // reading message using recv
    // recv(int socket, buffer, length, flags)
    // return: number of bytes or -1 if error
    
    
    char buffer[256]; // creating a message buffer to read from the server
    int result; 
    char* answer = (char*) malloc(256*sizeof(char));
    // infinite loop. Will exit at solve_math(buffer though)
    while (1){
    int read_status = recv(socket_desc, &buffer, sizeof(buffer), 0);
        if (read_status == -1){
            
        perror("Reading message unsuccessful");
        exit(-1);
        }
    printf("this yeah suppose to be a new message\n");
    printf("Read message. Number of bytes: %d. Message: %s \n", read_status, buffer);

    result = solve_math(buffer);


    // ---- SENDING ANSWER TO THE SERVER ----
    sprintf(answer, "CS230 %d\n", result); // convert integer to string
    printf("answer is %s\n", answer);
    send_msg(socket_desc, answer);

    printf("Ready for the next challenge. End of while loop\n");
    }
    printf("Closing client\n");
    close(socket_desc);
    
    return 0;

    //// read from the file descriptor socket
    //// read(int file_descriptor, char* buffer, sizeof(buffer))
    //// return: number of bytes otherwise return -1 and set errno
    //int msg_received = read(socket_desc, buffer, sizeof(buffer));
    //while (msg_received == 0){
    //if (msg_received == -1){
    //    perror("Reading message unsuccessful");
    //    exit(-1);
    //}
    //printf("keep reading msg\n");
    //}
    //printf("Read message. Number of bytes: %d. Message: %s \n", msg_received, buffer);

}

