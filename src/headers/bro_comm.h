#ifndef __bro_headers_comm_h
#define __bro_headers_comm_h

#include "bro_fist.h"

/*  In here we have all the functions used to control the communications between
 *  the BROFist Server and the Client, might it be Scicos or and "ad hoc"
 *  application created specifically.
 */


/*  bro_start_server is used to start the server by making it listen on a local
 *  socket for a communication by the client.
 *  server_sock:    The variable used to store the server's socket descriptor
 *                  used only at the end (please refer to bro_stop_server) to 
 *                  close the socket.
 *  client_sock:    The variable used to store the client's socket (Scicos or
 *                  another application) descriptor. Used to communicate with it
 *                  with the bro_server_fist function.
 */
int bro_start_server (int * server_sock, int * client_sock);

/*  bro_server_fist is the function that receives the Operational Fists from the
 *  client, sends them to the SPAM, receives the results of such Fists and then
 *  sends these results to the client.
 *  input_fist *:   Pointer to the bro_fist_t array containing all the Fists
 *                  that have to be sent to the SPAM
 *  output_fist *:  Pointer to the bro_fist_t array that will contain the
 *                  results.
 *  scicos_sock:    Socket Descriptor for the client connection. Normally is the
 *                  client_sock returned by the bro_start_server function.
 *  spam_sock:      Socket Descriptor for the SPAM connection. Normally is the
 *                  spam_sock returned by the bro_bt_connect_device function
 *                  (bro_bt.h)
 */
int bro_server_fist (const bro_fist_t * input_fist, bro_fist_t * out_fist, int scicos_sock, int spam_sock);

/*  bro_stop_server is used to stop the server, normally called on finalization. 
 *  server_sock:    The server socket descriptor.
 *  client_sock:    The client socket descriptor.
 */
int bro_stop_server (int server_sock, int client_sock);

#endif 
