#include <stdio.h>
#include "headers/BRO_spam_fists.h"
#include "headers/BRO_spam_client.h"
#include "headers/BRO_spam_pid.h"


/*--------------------------------------------------------------------------*/
/* OSEK declarations                                                        */
/*--------------------------------------------------------------------------*/
DeclareCounter(SysTimerCnt);
DeclareResource(lcd);
DeclareTask(BT_Logger);
DeclareTask(PID_Controller);
DeclareTask(Speed_Updater);
DeclareTask(DisplayTask);

/*--------------------------------------------------------------------------*/
/* LEJOS OSEK hooks                                                         */
/*--------------------------------------------------------------------------*/
void ecrobot_device_initialize()
{
    ecrobot_init_bt_slave("1234");
}


void ecrobot_device_terminate()
{
    nxt_motor_set_speed(NXT_PORT_A, 0, 1);
    nxt_motor_set_speed(NXT_PORT_B, 0, 1);
    nxt_motor_set_speed(NXT_PORT_C, 0, 1);

    ecrobot_term_bt_connection();
}

/*--------------------------------------------------------------------------*/
/* Function to be invoked from a category 2 interrupt                       */
/*--------------------------------------------------------------------------*/
void user_1ms_isr_type2(void)
{
    StatusType ercd;

    /*
     *  Increment OSEK Alarm System Timer Count
    */
    ercd = SignalCounter( SysTimerCnt );
    if ( ercd != E_OK ) {
        ShutdownOS( ercd );
    }
}

/*--------------------------------------------------------------------------*/
/* Task information:                                                        */
/* -----------------                                                        */
/* Name    : PID_Controller                                                 */
/* Priority: 3                                                              */
/* Typ     : EXTENDED TASK                                                  */
/* Schedule: preemptive                                                     */
/*--------------------------------------------------------------------------*/
TASK(PID_Controller)
{
    
    /*  We will now update the powers for each motor.
     *  If a motor is set as RAW_POWER then the first "powers" value is used
     *  directly for the speed update, while if a motor is set as PID_CONTROLLED
     *  the speed_ref is used in combination with the PID Controller, which
     *  computes the power needed to reach the target DegPS (Degrees Per Second)
     *  Oblviously it will be possible for the users to not install a motor,
     *  so we won't do anything with the ports on which there is nothing.
     */
    
    if (engines.first.speed_control_type != NOT_USING) {
        //  Updating speed errors
        update_errors(&engines.first);
        
        //  If we are controlling  it with PID we have to compute the power...
        if (engines.first.speed_control_type == PID_CONTROLLED) {
            update_powers(&engines.first);
        }
        //  Alternatively we will use the power set by the BT Task.
        
        //  Now we will set the powers using the data inside the motor's
        //  structure.
        nxt_motor_set_speed(engines.first.port, engines.first.powers[0], 1);
    };
    
    //  Doing the same thing for the second motor
    
    if (engines.second.speed_control_type != NOT_USING) {
        update_errors(&engines.second);
        if (engines.second.speed_control_type == PID_CONTROLLED) {
            update_powers(&engines.second);
        }
        nxt_motor_set_speed(engines.second.port, engines.second.powers[0], 1);
    };
    
    //  And, guess what? We are doing it even for the third motor.
    
    if (engines.third.speed_control_type != NOT_USING) {
        update_errors(&engines.third);
        if (engines.third.speed_control_type == PID_CONTROLLED) {
            update_powers(&engines.third);
        }
        nxt_motor_set_speed(engines.third.port, engines.third.powers[0], 1);
    };    

    TerminateTask();
}

TASK(Speed_Updater)
{
    //  Updating Speeds (And by that I mean the average DegPS for the motors)
    if (engines.first.speed_control_type != NOT_USING) {
        update_speed(&engines.first);
    }
    
    if (engines.second.speed_control_type != NOT_USING) {
        update_speed(&engines.second);
    }
    
    if (engines.third.speed_control_type != NOT_USING) {
        update_speed(&engines.third);
    }
    
    TerminateTask();
}

TASK(BT_Logger)
{
    U32 connect_status = 0;
    
    /*  Declaring two buffers for communication */
    bro_fist_t in_packet[BUFFER_SIZE];
    bro_fist_t out_packet[BUFFER_SIZE];

    /*  As you might know we have a problem here... :3
     *  That problem is that the BT device installed on the AT91SAM7 seems to
     *  have some speed problems with the receiving for the first data via BT.
     *  It needs ~40ms to get every kind of data (Even a uint32) so we will have
     *  to use some kind of pooling (not really pooling, because it would lock
     *  the NXT, mind you ;) ) and work on the data received only when some
     *  data, usually all of it (I won't tell you to read the device drivers
     *  for BlueTooth written for nxtOSEK, but the ecrobot_read_bt_packet checks
     *  if all the data declared in the first byte is present in the device's
     *  buffer.
     *
     *  No problem, with our drill we will pierce the Heavens!
     *  (And also with our BROFists, right?)
     */
    connect_status = ecrobot_read_bt_packet(&in_packet, sizeof(bro_fist_t)*BUFFER_SIZE);
    
    // Se sono arrivati dei dati...
    if (connect_status > 0) {
        
        bt_send((U8*)&out_packet, sizeof(bro_fist_t)*BUFFER_SIZE);
    }

    TerminateTask();
}

TASK(DisplayTask)
{
    ecrobot_status_monitor("BROFist Client");
    TerminateTask();
}
