/*
 * Matthew Diamond 2015
 * This file contains the driver function. Everything starts
 * and ends here.
 */

// Include header file
#include "main.hpp"

using namespace std;

/**********************
 * EXTERNAL VARIABLES *
 **********************/

int SAMPLE_RATE = 44100;
int BUFFER_SIZE;

int CURRENT_SAMPLE = 0;
int AUDIO_LENGTH = 44100 * 10;

SDL_Window *WINDOW;
SDL_Renderer *RENDERER;
int WINDOW_WIDTH = 500;
int WINDOW_HEIGHT = 100;

vector<Module *> modules;

/***********************
 * TESTING MODE TOGGLE *
 ***********************/

int testing = 0;

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * Run in testing mode. If all tests pass, return 1.
 * Otherwise, return 0;
 */
int testing_mode()
{
  return run_tests();
}

/*
 * Run in normal mode. If all objects create successfully,
 * return 1. Otherwise, return 0.
 */
int normal_mode()
{
  /**********************************
   * Initialize SDL and SDL objects *
   **********************************/

  cout << "Initializing SDL." << endl;

  // Initialize SDL with the video and audio subsystems
  if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)) { 
    cout << "Could not initialize SDL: " <<  SDL_GetError() << endl;
    return 0;
  }
  cout << "SDL initialized." << endl;

  // Initialize audio device
  if(!open_audio_device())
    return 0;
  cout << "Audio device opened." << endl;

  // Open a window
  if(!open_window())
    return 0;
  cout << "Window opened." << endl;

  // Create a renderer
  if(!create_renderer())
    return 0;
  cout << "Renderer created." << endl;

  /************************************************
   * Initialize output and begin processing audio *
   ************************************************/

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

  /************
   * Clean up *
   ************/

  // Destroy the graphics objects
  SDL_DestroyWindow(WINDOW);
  SDL_DestroyRenderer(RENDERER);

  // Quit SDL
  cout << "Quitting SDL." << endl;
  SDL_Quit();

  return 1;
}

/*****************
 * MAIN FUNCTION *
 *****************/

/*
 * Driver function for the whole program. This program 
 * will either run in test mode or normally. If run in test
 * mode this function will run a series of tests and then exit.
 * If run normally, this function will open the audio device,
 * initialize the output module, and then wait for user interaction.
 */
int main()
{
  int exit_status = 0;

  // If this is testing mode, just run the tests
  if(testing)
  {
    if(!testing_mode())
      exit_status = -1;
  }
  // If this is normal mode, open SDL, initialize necessary
  // objects, and begin processing audio and video
  else
  {
    if(!normal_mode())
      exit_status = -1;
  }

  // Return an exit status based on whether or not
  // the function called above succeeded and terminate
  cout << "Quitting..." << endl;
  return exit_status;
}