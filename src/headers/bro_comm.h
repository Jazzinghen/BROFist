#ifndef __bro_headers_comm_h
#define __bro_headers_comm_h

#include "bro_fist.h"

int bro_start_server (int * server_sock, int * client_sock);

int bro_server_fist (bro_fist_t * input_fist, bro_fist_t * out_fist, int scicos_sock/*, int spam_sock*/);

int bro_stop_server (int server_sock, int client_sock);

#endif 
