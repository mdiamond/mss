/*
 * Matthew Diamond 2015
 * The header for the Timer class. This class is used to measure
 * framerate and time how long functions are taking. This file
 * defines the class.
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
		// Members
		Uint32 last_checked_ms;

		// Constructor and desctructor
		Timer();
		~Timer();

		// Member functions
		void start();
		Uint32 check_time_elapsed();
};

#endif
