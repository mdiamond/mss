/*
 * Matthew Diamond 2015
 * This file contains the driver function. Everything starts
 * and ends here. It initializes all external variables,
 * then either runs the testing functions or starts SDL and
 * begins processing.
 */

// Include header file
#include "main.hpp"

using namespace std;

/**********************
 * EXTERNAL VARIABLES *
 **********************/

// Audio information
int SAMPLE_RATE = 44100;
int BUFFER_SIZE;

int CURRENT_SAMPLE = 0;
int AUDIO_LENGTH = 44100 * 10;

// SDL Window and renderer
SDL_Window *WINDOW;
SDL_Renderer *RENDERER;

// Module dimensions and amount of modules per page
int MODULE_WIDTH = 150;
int MODULE_HEIGHT = 250;
int MODULES_PER_ROW = 6;
int MODULES_PER_COLUMN = 3;
int MODULE_SPACING = 1;
int MODULE_BORDER_WIDTH = 2;

// Window dimensions
int WINDOW_WIDTH = (MODULES_PER_ROW * MODULE_WIDTH) +
                   (MODULE_SPACING * MODULES_PER_ROW);
int WINDOW_HEIGHT = (MODULES_PER_COLUMN * MODULE_HEIGHT) +
                    (MODULE_SPACING * MODULES_PER_COLUMN);

vector<Module *> MODULES;
int MODULES_CHANGED = 1;

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

  // Open a window
  if(!open_window())
    return 0;

  // Create a renderer
  if(!create_renderer())
    return 0;

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
    cout << "AFTER LOCK AUDIO" << endl;
    draw_surface();
    cout << "AFTER DRAW SURFACE" << endl;
    SDL_UpdateWindowSurface(WINDOW);
    cout << "AFTER UPDATE WINDOW SURFACE" << endl;
    SDL_UnlockAudio();
    cout << "AFTER UNLOCK AUDIO" << endl;
    SDL_Delay(1000 / 15);
    cout << "AFTER DELAY" << endl;
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
