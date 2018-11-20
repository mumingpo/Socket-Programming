# Socket Programming

## Server Config
IP Address: 128.119.243.147

Port number: 27993

## Protocol Requirements
To capture the flag from the math bot server you must combine your understanding of the socket API functions and an application-layer protocol known as math speak which is as follows:

When you first connect to the server you must identify yourself. In particular, you must send the following string:

cs230 HELLO <NETID>@umass.edu\n

This string must be exactly as we describe above - network protocols are very specific. You must replace <NETID> with your UMass NetID. By sending the identification string it will initiate the math bot server to start sending you math problems. You will immediately receive a "status" message with the following format:

cs230 STATUS NUM OP NUM\n

This status message includes a simple arithmetic operation. An example of an actual message is:

cs230 STATUS 505 * 700\n

You will need to implement functionality in your client that will compute the math problem provided in the server's status message. After you do that you need to send a response back to the math bot server that is formatted like this:

cs230 <ANSWER>\n

You must replace <ANSWER> with the answer to the math problem. Here is an example:

cs230 353500\n

With your response back to the server, the server will then repeatedly send you hundreds of math problems that your client must solve. Your client will need to solve each of the status math problems until you receive the response with the flag:

cs230 <FLAG> BYE\n

The <FLAG> is a long hash value. Here is an example of the final message you will receive from the server before it disconnects from your client:

cs230 7c5ee45183d657f5148fd4bbabb6615128ec32699164980be7b8b451fd9ac0c3 BYE

If you are able to "capture the flag" you have completed this assignment successfully. You still need to submit it to Gradescope though.

