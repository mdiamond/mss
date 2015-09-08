/*
 * Matthew Diamond 2015
 * Member functions for the Oscillator class
 */

// Included libraries
#include <string>

// Included files
#include "main.hpp"

// Included classes
#include "Module.hpp"
#include "Oscillator.hpp"

using namespace std;

/*
 * Constructor
 */
Oscillator::Oscillator(string *_name)
  : name(*_name)
{
  type = OSCILLATOR;
  frequency = 0;
  phase = 0;
  amplitude = 1;
  input = NULL;
  output = new vector<float>(BUFFER_SIZE, 0);
}

/*
 * Dummy function
 */
Oscillator::~Oscillator(void)
{

}

/*
 * Process all dependencies, then
 * fill the output buffer with a waveform given
 * the data contained within this class and the
 * audio device information.
 */
void Oscillator::process()
{
  // Check for any dependencies for frequency modulation
  process_depends();
  // Calculate an amplitude for each sample
  for(int i = 0; i < BUFFER_SIZE; i ++)
  {
    // Calculate and store the current samples amplitude
    // based on phase
    (*output)[i] = amplitude * sin(phase);
    // Calculate phase for the next sample
    phase += ((2 * M_PI * frequency) / SAMPLE_RATE);
    if(phase > (2 * M_PI))
      phase -= (2 * M_PI);
  }
}