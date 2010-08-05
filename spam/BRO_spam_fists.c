#include "headers/BRO_spam_fists.h"

//  :3 Listen, here we will write the function that will decode only ONE order and will return some data, IF it's a sensors packet...
static void
decode_bro_input (bro_fist_t * input_packet, bro_fist_t * output_packet)
{

}


//  Function to decode a LIST of FISTS and do things accordingly... :3 
void decode_bro_fists (bro_fist_t * orders, bro_fist_t * response, struct motors)
{
    int i;
    
    for (i = 0; i < BUFFER_SIZE; i ++) {
        
    }
};
