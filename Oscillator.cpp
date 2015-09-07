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

Oscillator::Oscillator(string *_name)
  : name(*_name)
{
  frequency = 0;
  type = OSCILLATOR;
  input = NULL;
  output = new vector<float>(BUFFER_SIZE / 2, 0);
}

Oscillator::~Oscillator(void)
{

}

void Oscillator::process(int num_samples)
{
  process_depends(num_samples);
  // For each sample in the output buffer,
  // calculate an amplitude based on the
  // current sample and place it in the buffer
  for(int i = 0; i < num_samples; i ++)
  {
    float wave_fraction = ((CURRENT_SAMPLE + i) / (SAMPLE_RATE / (frequency * 2)));
    float sample_amplitude = sin(M_PI * wave_fraction);
    
    (*output)[i] = sample_amplitude;
  }
}