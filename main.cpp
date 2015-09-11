/*
 * Matthew Diamond 2015
 * This file contains the driver function. Everything starts
 * and ends here.
 */

// Include header file
#include "main.hpp"

using namespace std;

int SAMPLE_RATE = 44100;
int BUFFER_SIZE;

int CURRENT_SAMPLE = 0;
int AUDIO_LENGTH = 44100 * 10;

SDL_Window *WINDOW;
SDL_Renderer *RENDERER;
int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 100;

vector<Module *> modules;

int testing = 0;

int testing_mode()
{
  if(run_tests())
    return 0;
  else
    return -1;
}

int normal_mode()
{
  cout << "Initializing SDL." << endl;

  // Initialize SDL with the video and audio subsystems
  if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)) { 
    cout << "Could not initialize SDL: " <<  SDL_GetError() << endl;
    return -1;
  }
  cout << "SDL initialized." << endl;

  // Initialize audio device
  if(!open_audio_device())
    return -1;
  cout << "Audio device opened." << endl;

  // Open a window
  if(!open_window())
    return -1;
  cout << "Window opened." << endl;

  // Create a renderer
  if(!create_renderer())
    return -1;
  cout << "Renderer created." << endl;

  // Initialize the output module
  initialize_output();

  // Unpause the audio
  cout << "Unpausing audio." << endl;
  SDL_PauseAudio(0);
  cout << "Audio unpaused." << endl;

  // While there's still sound to play, just wait
  while(AUDIO_LENGTH > 0)
  {
    SDL_LockAudio();
    draw_surface();
    SDL_UpdateWindowSurface(WINDOW);
    SDL_UnlockAudio();
    SDL_Delay(1000 / 30);
  }

  // Destroy the graphics objects
  SDL_DestroyWindow(WINDOW);
  SDL_DestroyRenderer(RENDERER);

  // Quit SDL
  cout << "Quitting SDL." << endl;
  SDL_Quit();
}

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
    testing_mode();
  }
  else
  {
    normal_mode();
  }

  // Terminate
  cout << "Quitting..." << endl;
  return 0;
}