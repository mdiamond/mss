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

long long unsigned int time = 0;

const int AMPLITUDE = 28000;
const int FREQUENCY = 44100;

struct ToneObject
{
  double freq;
  int samplesLeft;
};

class Square
{
  double v;
  queue<ToneObject> tones;
public:
  Square();
  void beep(double freq, int duration);
  void generateSamples(Sint16 *stream, int length);
  void play(double, int);
  void wait();
};

void audio_callback(void *, Uint8 *, int);

Square::Square()
{
  SDL_AudioSpec desiredSpec;

  desiredSpec.freq = FREQUENCY;
  desiredSpec.format = AUDIO_S16SYS;
  desiredSpec.channels = 2;
  desiredSpec.samples = 2048;
  desiredSpec.callback = audio_callback;
  desiredSpec.userdata = this;

  SDL_AudioSpec obtainedSpec;

  SDL_OpenAudio(&desiredSpec, &obtainedSpec);

  SDL_PauseAudio(0);
}

void Square::generateSamples(Sint16 *stream, int length)
{
  int i = 0;
  while(i < length)
  {
    if(tones.empty())
    {
      while(i < length)
      {
        stream[i] = 0;
        i ++;
      }
      return;
    }
    ToneObject& to = tones.front();

    int samplesLeft = std::min(i + to.samplesLeft, length);
    to.samplesLeft -= samplesLeft - i;

    while(i < samplesLeft)
    {
      stream[i] = AMPLITUDE * sin(v * 2 * M_PI / FREQUENCY);
      i ++;
      v += to.freq;
    }

    if(to.samplesLeft == 0)
    {
      tones.pop();
    }
  }
}

void Square::play(double freq, int duration)
{
  ToneObject to;
  to.freq = freq;
  to.samplesLeft = duration * FREQUENCY / 1000;

  SDL_LockAudio();
  tones.push(to);
  SDL_UnlockAudio();
}

void Square::wait()
{
  int size;
  do
  {
    SDL_Delay(20);
    SDL_LockAudio();
    size = tones.size();
    SDL_UnlockAudio();
  } while (size > 0);
}

void audio_callback(void *_square, Uint8 *_stream, int _length)
{
  Sint16 *stream = (Sint16*) _stream;
  int length = _length / 2;
  Square *square = (Square *) _square;

  square->generateSamples(stream, length);
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
    exit(1);
  }

  cout << "SDL initialized." << endl;

  Square s;
  s.play(440, 1000);
  s.wait();
  s.play(523.25, 1000);
  s.wait();

  // Quit SDL
  cout << "Quitting SDL." << endl;
  SDL_Quit();

  // Terminate
  cout << "Quitting..." << endl;
  return 0;
}
