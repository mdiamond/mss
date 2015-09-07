/*
 * Matthew Diamond 2015
 * This file contains the driver function. Everything starts
 * and ends here.
 */

// Included libraries
#include <cmath>
#include <iostream>
#include <queue>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_audio.h"

// Included files
#include "main.hpp"
#include "signal_processing.hpp"
#include "tests.hpp"

// Included classes
#include "Module.hpp"
#include "Output.hpp"
#include "Oscillator.hpp"

using namespace std;
  
int SAMPLE_RATE = 44100;
unsigned long long int CURRENT_SAMPLE = 0;
int AUDIO_LENGTH = 44100;
float FREQUENCY = 440;
int BUFFER_SIZE;

vector<Module *> modules;

int testing = 0;

/*
 * Audio callback which triggers the generation of samples.
 */
void audio_callback(void *userdata, Uint8 *_buffer, int length)
{
  // 4 bytes per float, one left and one right
  int num_samples = length / 8;

  float *buffer = (float *) _buffer;

  // If there is no audio to play
  if(AUDIO_LENGTH == 0)
    return;

  // Process audio for the output module
  modules[0]->process(num_samples);

  // Fetch the output module's latest processed audio
  // and insert it into the buffer
  vector<float> *l = ((Output *) modules[0])->input_l;
  vector<float> *r = ((Output *) modules[0])->input_r;

  int index = 0;
  for(int i = 0; i < l->size(); i ++)
  {
    cout << (*l)[i] << ", ";
    buffer[index] = (*l)[i];
    index ++;
    buffer[index] = (*r)[i];
    index ++;
  }

  // Increment the current sample by the number
  // of samples just processed
  AUDIO_LENGTH -= length / 2;
  CURRENT_SAMPLE += length / 2;  

  //fill_buffer(buffer, length);
}

/*
 * Driver function for the whole program.
 */
int main()
{
  if(testing)
  {
    if(run_tests())
      return 0;
    else
      return 1;
  }
  else
  {
    cout << "Initializing SDL." << endl;

    // Initialize SDL with the video and audio subsystems
    if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)) { 
      cout << "Could not initialize SDL: " <<  SDL_GetError() << endl;
      return 1;
    }
    cout << "SDL initialized." << endl;

    // Initialize audio device
    if(!open_audio_device())
    {
      cout << "Could not open the audio device: " << SDL_GetError() << endl;
      return 1;
    }
    cout << "Audio device opened." << endl;

    // Create the output module
    Output *output = new Output();
    modules.push_back(output);
    // Create an oscillator module
    string oscillator_1_name = "oscillator 1";
    Oscillator *oscillator_1 = new Oscillator(&oscillator_1_name);
    modules.push_back(oscillator_1);
    // Set the inputs and outputs
    output->depends.push_back(oscillator_1);
    output->input_l = oscillator_1->output;
    output->input_r = oscillator_1->output;
    // Set the oscillator frequency
    oscillator_1->frequency = 440;

    cout << "Unpausing audio." << endl;
    SDL_PauseAudio(0);
    cout << "Audio unpaused." << endl;

    while(AUDIO_LENGTH > 0)
    {}

    // Quit SDL
    cout << "Quitting SDL." << endl;
    SDL_Quit();
  }

  // Terminate
  cout << "Quitting..." << endl;
  return 0;
}