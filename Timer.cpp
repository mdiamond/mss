/*
 * Matthew Diamond 2015
 * Member functions for the Timer class.
 */

// Include header file
#include "Timer.hpp"

 using namespace std;

/***************************
 * MODULE MEMBER FUNCTIONS *
 ***************************/

/*
 * Dummy function
 */
Timer::Timer()
{

}

/*
 * Dummy function
 */
Timer::~Timer()
{

}

void Timer::start()
{
  last_checked_ms = SDL_GetTicks();
}

Uint32 Timer::check_time_elapsed()
{
  Uint32 time_elapsed_ms = SDL_GetTicks() - last_checked_ms;
  last_checked_ms = SDL_GetTicks();
  return time_elapsed_ms;
}