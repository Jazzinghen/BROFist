#include "headers/bro_fist.h"
#include "headers/bro_comm.h"
#include "headers/bro_opts.h"

#define BRO_END_COMMUNICATION 255

int 
main (int argc, char *argv[])
{
    int bro_server_socket = -1;
    int bro_client_socket = -1;
    //int spam_socket = -1;
    bro_fist_t in_packet, out_packet;
    
    bro_opts_t options;
    
    
    if (opts_parse (&options, argc, argv) == -1) {
        return 0;
    };
    
    bro_start_server (&bro_server_socket, &bro_client_socket);
    
    //bro_bt_connect_device (&spam_socket, options.mac);
    
    do {
        // TODO: Fare il loop di comunicazione con, magari, una word per il blocco
        bro_server_fist(&in_packet, &out_packet, bro_client_socket);
        
    } while (in_packet.operation != BRO_END_COMMUNICATION);
    
    //bro_bt_close_connection(spam_socket);
    
    bro_stop_server(bro_server_socket, bro_client_socket);
    
    return 0;
}
