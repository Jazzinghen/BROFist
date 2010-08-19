#ifndef __bro_headers_fist_h
#define __bro_headers_fist_h


#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// Definitions for the Cient-Server model
#define SERVER_PATH             "/tmp/BROFist"
#define BUFFER_SIZE             7
#define BRO_END_COMMUNICATION   255

// Definitions for Ports
#define MOTOR_A                 5
#define MOTOR_B                 6
#define MOTOR_C                 7

#define PORT_1                  1
#define PORT_2                  2
#define PORT_3                  3
#define PORT_4                  4

// Definitions for operations
#define LIGHT_SENSOR            1
#define TOUCH_SENSOR            2
#define SOUND_SENSOR            3
#define RADAR_SENSOR            4

#define TACHO_COUNT             6   // Get RAW Tacho count from Motor
#define AVG_SPEED               7   // Get Average Speed as computed by the Client

#define SET_SPEED               8   // Set speed to be computed by the PID on the NXT
#define SET_POWER               9   // Set RAW power for the Motor


typedef struct {
    uint8_t operation;
    uint8_t port;
    float data;
}
#ifndef DOXYGEN
__attribute__((__packed__))
#endif
bro_fist_t;

typedef struct {
    uint16_t size;
    bro_fist_t packets[BUFFER_SIZE];
}
#ifndef DOXYGEN
__attribute__((__packed__))
#endif
bro_spam_fists_t;

#endif 
