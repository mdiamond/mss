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
int BUFFER_SIZE;

int AUDIO_LENGTH = 44100 * 10;

vector<Module *> modules;

int testing = 0;

/*
 * Driver function for the whole program. This program 
 * will either run in test mode or normally. If run in test
 * mode this function will run a series of tests and then exit.
 * If run normally, this function will open the audio device,
 * initialize the output module, and then wait for user interaction.
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

    // Create two oscillator modules
    string oscillator_1_name = "oscillator 1";
    Oscillator *oscillator_1 = new Oscillator(&oscillator_1_name);
    modules.push_back(oscillator_1);
    // string oscillator_2_name = "oscillator_2";
    // Oscillator  *oscillator_2 = new Oscillator(&oscillator_2_name);
    // modules.push_back(oscillator_2);

    // Set the inputs and outputs
    output->depends.push_back(oscillator_1);
    // output->depends.push_back(oscillator_2);
    output->input_l = oscillator_1->output;
    output->input_r = oscillator_1->output;

    // Set the oscillator frequencies
    oscillator_1->frequency = 440;
    // oscillator_2->frequency = 1;

    cout << "Unpausing audio." << endl;
    SDL_PauseAudio(0);
    cout << "Audio unpaused." << endl;

    while(AUDIO_LENGTH > 0)
    {

    }
    
    // Quit SDL
    cout << "Quitting SDL." << endl;
    SDL_Quit();
  }

  // Terminate
  cout << "Quitting..." << endl;
  return 0;
}