#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "strexpeval.c"

typedef struct client Client;

struct client{
  int fd; // file descriptor for the socket
  struct sockaddr address; // the sockaddr object as defined in netinet/in.h
};

// creates and connects a client with the correct address and port. returns 0 if successful.
// accepts the string representation for address (ipv4 format) and port
int client_init(char * address_s, char * port_s, Client * client);

// sends a message through the client. returns 0 if successful.
// although this shouldn't modify client, client is passed as a reference because copying takes time
int client_send_msg(void * msg, size_t length, Client * client);

// reads a message from the network, returns 0 upon successful exit.
int client_recv_msg(void * buffer, size_t max_length, Client * client);

/* should be done by the driver
  // runs the client and sets the flag. returns 0 upon successful exit.
  int client_run(char ** flag, Client * client);
*/

// closes connection and frees the contents of the client, if there is any.
void client_close(Client * client);