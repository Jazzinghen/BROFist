#ifndef __bro_headers_comm_h
#define __bro_headers_comm_h

int bro_start_server (int * server_sock, int * client_sock);

int bro_server_fist ();

int bro_stop_server (int server_sock, int client_sock);
