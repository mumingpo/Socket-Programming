#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "client.c"
#include "strexpeval.h"

#define MAX_MSG_SIZE 200

// SQ 11/15 driver

/* TODO:
  Step 1: Open a TCP stream socket
  Step 2: Connect to the remote server on the specified port number
  Step 3: Send your SPIRE ID to the server in the following format: NETID@umass.edu
  Step 4: Receive the first math problem
  Step 5: Send the CORRECT solution to the server (Server will drop connection if it is wrong)
  Step 6: Continue steps 4 and 5 for a random amount of times (No less than 300, but no more than 2000)
*/

/* break down
  driver: accept input and initiate client, loop until 
*/


int main(int argc, char *argv[]){
  if (argc != 4){
    printf("usage: ./client netid@umass.edu 27993 128.119.243.147\n");
    exit(1);
  }

  char * email = argv[1];
  Client client;
  
  if (client_init(argv[3], argv[2], &client) != 0){
    printf("Error in initializing client\n");
    return 1;
  }
  
  int result;
  char msg_buffer[MAX_MSG_SIZE];
  char exp_buffer[MAX_MSG_SIZE];
  size_t msg_len;
  
  // hello
  snprintf(msg_buffer, MAX_MSG_SIZE, "cs230 HELLO %s\n", email);
  // turns out the server doesn't want the \0!
  msg_len = strlen(msg_buffer)/* + 1*/; // strlen doesn't include the \0 at the end
  if (client_send_msg((void *) msg_buffer, msg_len, &client) != 0){
    printf("Error in sending message\n");
    return 1;
  }

  while (1){
    // receive message
    memset(msg_buffer, 0, MAX_MSG_SIZE);//debug
    if (client_recv_msg((void *) msg_buffer, MAX_MSG_SIZE, &client) != 0){
      printf("Error in receiving message\n");
      return 1;
    }
    msg_len = strlen(msg_buffer) + 1;
    assert(strncmp(msg_buffer, "cs230 ", 6) == 0);
    assert(msg_buffer[msg_len - 2] == '\n');
    
    // determine if mission accomplished
    // 6 = the string minus "cs230 "
    if (strncmp(msg_buffer + 6, "STATUS ", 7) != 0){
      break;
    }
    
    // evaluate expression
    // truncate the \n. msg_buffer is not going to be used later so this is justified
    // 13 = the string minus "cs230 STATUS "
    msg_buffer[msg_len - 2] = '\0';
    result = str_exp_eval(msg_buffer + 13);
    
    // submit message
    memset(msg_buffer, 0, MAX_MSG_SIZE);
    snprintf(msg_buffer, MAX_MSG_SIZE, "cs230 %d\n", result);
    // turns out the server doesn't want the \0!
    msg_len = strlen(msg_buffer)/* + 1*/;
    if (client_send_msg((void *) msg_buffer, msg_len, &client) != 0){
      printf("Error in sending message\n");
      return 1;
    }
  }
  
  // extract and print flag
  // -6 = " BYE\n\0"
  // 6 = the string minus "cs230 "
  msg_buffer[msg_len - 6] = '\0';
  printf("flag: %s\n", msg_buffer + 6);
  
  client_close(&client);
  
  return 0;
}