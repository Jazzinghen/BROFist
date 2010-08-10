#ifndef __bro_headers_spam_pid_h
#define __bro_headers_spam_pid_h
#include "BRO_spam_client.h"

//  This display function is here only for retrocompatibility and for debugging purposes
void disp (int row, char *str, int val);

//  Average speed
F32 avg(motor_t *mot);

//  
void update_speed (motor_t *mot);

void update_errors (motor_t *mot);

void update_powers (motor_t *mot);

#endif
