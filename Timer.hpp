/*
 * Matthew Diamond 2015
 * The header for the Timer class. This class is used to measure
 * framerate and time how long functions are taking.
 */

#ifndef synth_timer_h
#define synth_timer_h

/************
 * INCLUDES *
 ************/

// No includes necessary

/**************************
 * TIMER CLASS DEFINITION *
 **************************/

class Timer
{
    public:
        Uint32 last_checked_ms;
        Timer();
        ~Timer();
        void start();
        Uint32 check_time_elapsed();
};

#endif
