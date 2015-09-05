/*
 * Matthew Diamond 2015
 * This file contains the driver function. Everything starts
 * and ends here.
 */

// Included libraries
#include <iostream>
#include <cmath>
#include <queue>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_audio.h"

using namespace std;

unsigned int CURRENT_SAMPLE = 0;

int SAMPLE_RATE = 44100;

int AUDIO_LENGTH = 44100;

float FREQUENCY = 440;

void audio_callback(void *, Uint8 *, int);

void fill_buffer(Uint8 *_buffer, int length)
{
  // Float samples are 32 bits, 4 bytes * 2 (stereo)
  // is 8 bytes per unique sample, 4 bytes for left, 4 for right
  int num_unique_samples = length / 8;
  int num_samples = length / 4;
  // Cast the buffer pointer to a float pointer to
  // allow inserting floats into its memory locations
  float *buffer = (float *) _buffer;
  // Start at the first sample in the buffer
  int index = 0;
  while(num_unique_samples > 0)
  {
    float wave_fraction = (CURRENT_SAMPLE / (SAMPLE_RATE / (FREQUENCY * 2)));
    float sample_amplitude = sin(M_PI * wave_fraction);
    buffer[index] = sample_amplitude;
    buffer[index + 1] = sample_amplitude;
    index += 2;
    CURRENT_SAMPLE ++;
    AUDIO_LENGTH --;
    num_unique_samples --;
  }
}

/*
 * Audio callback which triggers the generation of samples.
 */
void audio_callback(void *userdata, Uint8 *buffer, int length)
{
  // If there is no audio to play
  if(AUDIO_LENGTH == 0)
    return;

  //cout << "Callback requesting " << length << " bytes." << endl;
  fill_buffer(buffer, length);
  //cout << "Buffer filled." << endl;
}

/*
 * Open the audio device with the specified configuration
 */
int open_audio_device()
{
  SDL_AudioSpec wanted, obtained;
  
  wanted.freq = 44100;
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

  // Terminate
  cout << "Quitting..." << endl;
  return 0;
}
