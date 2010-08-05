#include "headers/BRO_spam_pid.h"

/*
 * Power Saturation function. Simply limits the motors' powers to given upper
 * and lower limits.
 */
static inline
F32 saturate (F32 sig, F32 lowerl, F32 upperl)
{
        if (sig >= upperl) {
                return upperl;
        } else if (sig <= lowerl) {
                return lowerl;
        } else {
                return sig;
        }
}

/*
 * Simple String with Value display function.
 */
void disp (int row, char *str, int val)
{
        display_goto_xy(0, row);
        display_string(str);
        display_int(val, 0);
        display_update();
}

/*
 * Function to compute the motors' average speed.
 */
F32 avg(motor_t *mot)
{
        U8 j;
        F32 sum = 0;

        for (j=0; j<SAMP_NUM; j++) {
                sum += mot->speeds[j];
        };

        return (sum / SAMP_NUM);
};

//  Function to Update a Motor's Speed Readings.
void update_speed (motor_t *mot)
{
        S32 deltarev;   //  Delta value of the Motor's Revolutions
        S32 deltatime;  //  Delta value of the Motor's Sys Time

        //  Increasing Counters for the circular array
        mot->previous_counter = mot->counter;
        (mot->counter)++;
        if (mot->counter >= SAMP_NUM) {
                mot->counter=0;
        };

        //  Getting last Revolution and Last System Time
        mot->revolutions[mot->counter] = nxt_motor_get_count(mot->port);
        mot->times[mot->counter] = systick_get_ms();

        //  Computing Deltas and then using them to get the instant speed
        deltatime = mot->times[mot->counter] - mot->times[mot->previous_counter];
        deltarev = mot->revolutions[mot->counter] -
                   mot->revolutions[mot->previous_counter];
        mot->speeds[mot->counter] = deltarev * 1000 /deltatime;

};

//  Function to Update a Motor's speed errors
void update_errors (motor_t *mot)
{
        //  Swapping Speed Errors
        mot->speed_errors[2] = mot->speed_errors[1];
        mot->speed_errors[1] = mot->speed_errors[0];
        //  Updating Last Speed Error
        mot->speed_errors[0] = mot->speed_ref - avg(mot);
}

//  Function to Update a Motor's powers. In here we use the PID Controller. :3
void update_powers (motor_t *mot)
{
        //  Swapping Motor's Powers
        mot->powers[2] = mot->powers[1];
        mot->powers[1] = mot->powers[0];
        //  Computing next power using the PID Controller
        mot->powers[0] = (K * ((4 + Ki * T * T + 2 * Kp * T) * mot->speed_errors[0] +
                               (-8 + 2 * Ki * (T * T)) * mot->speed_errors[1] +
                               (-2 * Kp * T + 4 + Ki * T * T) * mot->speed_errors[2]) +
                          (8 * mot->powers[1] - (-2 * P1 * T + 4) * mot->powers[2]))
                         / (2 * P1 * T +4); // OMG, Longest Formula EVAR!
        //  Saturating the result to the maximum and minimum power of the motor.
        mot->powers[0] = saturate(mot->powers[0],-100.0F,100.0F);
}
