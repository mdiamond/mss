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

int FREQUENCY = 44100;

int AUDIO_LENGTH = FREQUENCY;

void audio_callback(void *, Uint8 *, int);

void fill_buffer(Uint8 *_stream, int length)
{
  float *stream = (float *) _stream;
  float freq = 440;
  for(int i = 0; i < length; i += 2)
  {
    int current_sample = CURRENT_SAMPLE + (i / 2);
    int wave_fraction = (current_sample / (FREQUENCY / (freq * 2)));
    int sample_amplitude = sin(M_PI * wave_fraction);
    stream[i] = sample_amplitude;
    stream[i + 1] = sample_amplitude;
  }

  CURRENT_SAMPLE += length / 2;
  AUDIO_LENGTH -= length / 2;
}

/*
 * Audio callback which triggers the generation of samples.
 */
void audio_callback(void *userdata, Uint8 *stream, int length)
{
  // If there is no audio to play
  if(AUDIO_LENGTH = 0)
    return;

  cout << "Callback requesting " << length << " samples." << endl;
  fill_buffer(stream, length);
  cout << "Buffer filled." << endl;
}

/*
 * Open the audio device with the specified configuration
 */
int open_audio_device()
{
  SDL_AudioSpec wanted;
  
  wanted.freq = 44100;
  wanted.format = AUDIO_F32SYS;
  wanted.channels = 2;
  wanted.samples = 2048;
  wanted.callback = audio_callback;
  wanted.userdata = NULL;

  if(SDL_OpenAudio(&wanted, NULL) < 0)
    return 0;

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
  if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)==-1)) { 
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

  SDL_PauseAudio(0);
  while(AUDIO_LENGTH > 0)
    SDL_Delay(1);

  // Quit SDL
  cout << "Quitting SDL." << endl;
  SDL_Quit();

  // Terminate
  cout << "Quitting..." << endl;
  return 0;
}
