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
    bro_fist_t in_packet[BUFFER_SIZE];
    bro_fist_t out_packet[BUFFER_SIZE];
    
    int i, comm_res;
    
    bro_opts_t options;
    
    srand ( time(NULL) );
    
    
    if (bro_opts_parse (&options, argc, argv) == -1) {
        return 0;
    };
    
    bro_start_server (&bro_server_socket, &bro_client_socket);
    
    //bro_bt_connect_device (&spam_socket, options.mac);
    
    do {
        // TODO: Fare il loop di comunicazione con, magari, una word per il blocco
        comm_res = bro_server_fist(in_packet, out_packet, bro_client_socket);
        
        for (i = 0; i < BUFFER_SIZE; i++) {
            printf("Istruzione %i:\n", i+1);
            printf("\tOperazione: %i\n", in_packet[i].operation);
            printf("\tPorta: %i\n", in_packet[i].port);
            printf("\tDati: %.2f\n", in_packet[i].data);
            printf("\tDati settati: %.2f\n\n", out_packet[i].data);
        }
        
    } while (in_packet[0].operation != BRO_END_COMMUNICATION || comm_res < 0);
    
    //bro_bt_close_connection(spam_socket);
    
    bro_stop_server(bro_server_socket, bro_client_socket);
    
    return 0;
}
