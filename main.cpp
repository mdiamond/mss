/*
 * Matthew Diamond 2015
 * This file contains the driver function. Everything starts
 * and ends here.
 */

// Included libraries
#include <cmath>
#include <iostream>
#include <queue>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_audio.h"

// Included files
#include "main.hpp"
#include "signal_processing.hpp"
#include "tests.hpp"
  
int SAMPLE_RATE = 44100;
unsigned long long int CURRENT_SAMPLE = 0;
int AUDIO_LENGTH = 44100;
float FREQUENCY = 440;

int testing = 1;

using namespace std;

/*
 * Audio callback which triggers the generation of samples.
 */
void audio_callback(void *userdata, Uint8 *buffer, int length)
{
  // If there is no audio to play
  if(AUDIO_LENGTH == 0)
    return;

  fill_buffer(buffer, length);
}

/*
 * Open the audio device with the specified configuration
 */
int open_audio_device()
{
  SDL_AudioSpec wanted, obtained;
  
  wanted.freq = SAMPLE_RATE;
  wanted.format = AUDIO_F32SYS;
  wanted.channels = 2;
  wanted.samples = 512;
  wanted.callback = audio_callback;
  wanted.userdata = NULL;

  if(SDL_OpenAudio(&wanted, &obtained) == -1)
    return 0;

  cout << "Audio details:" << endl;
  cout << "Sample rate: " << obtained.freq << endl;
  cout << "Format: " << obtained.format << endl;
  cout << "Channels: " << obtained.channels << endl;
  cout << "Buffer size: " << obtained.samples << endl;

  // Return success
  return 1;
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
