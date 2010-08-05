#ifndef __bro_headers_spam_client_h
#define __bro_headers_spam_client_h

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../src/headers/bro_fist.h"
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/*--------------------------------------------------------------------------*/
/* Definitions                                                              */
/*--------------------------------------------------------------------------*/

/*  It's a shame, but it's required to declare on which port are connected the
 *  sonar or the light sensor. It's messy but there are some initialisation that
 *  have to be performed even BEFORE the first packet. XD
 */
 
#define SONAR_PORT      NXT_PORT_S1 /* Simply put in here which port is            */
#define LIGHT_PORT      NXT_PORT_S2 /* connected which sensor. OR 0 if there isn't */
                                    /* that particular sensor. :3 (You'll see why) */
                                    
#define PID_CONTROLLED  1           //
#define RAW_POWER       0           //
#define NOT_USING       -1          // This is the initial condition and if not
                                    // used... :3

#define K               0.8F        // Constant K for PID
#define Kp              4.117F      // Constant Kp for PID
#define Ki              15.98F      // Constant Ki for PID
#define Kd              1           // Constant Kd for PID
#define P1              40          //  
#define T               0.005F      // Speed of a single reading (In seconds)
#define SAMP_NUM        10          // Number of Samples

/* Defining Motor Variables Structure */

typedef struct {

        // Is it controlled via PID or RAW power?
        S8 speed_control_type;

        // Target speed/power for motor
        U32 speed_ref;

        //  Motor's Revolutions containers
        U32 revolutions[SAMP_NUM];  // Last SAMP_NUM tacho counts.

        //  Motor's Times Containers
        U32 times[SAMP_NUM];        // Last SAMP_NUM system times.

        //  Speed Readings
        F32 speeds[SAMP_NUM];       // Last SAMP_NUM motor's speed

        //  Motor's Powers
        F32 powers[3];              // Last three Motor Input Powers.

        //  Speed Errors
        F32 speed_errors[3];        // Last three Speed Errors.

        //  Motor's Port
        U8  port;                   // Simply the motor's power. Nothing hard. ;)

        //  Miscellaneous Counters
        U32 counter;                // Circular Array Counter
        S32 previous_counter;       // Previous Slot in the Circular Array

} motor_t;

struct {
    motor_t first,
    motor_t second,
    motor_t third
} engines = {
    {.port = NXT_PORT_A, .speed_control_type = NOT_USING, .speed_ref = 0},
    {.port = NXT_PORT_B, .speed_control_type = NOT_USING, .speed_ref = 0},
    {.port = NXT_PORT_C, .speed_control_type = NOT_USING, .speed_ref = 0}
};

#endif
