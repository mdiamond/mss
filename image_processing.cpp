/*
 * Matthew Diamond 2015
 * This file contains all of the basic signal processing functions.
 * It also contains helper functions for the callback function
 * that are utilized to fill the audio buffer.
 */

// Included libraries
#include <iostream>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "main.hpp"
#include "signal_processing.hpp"

// Included classes
#include "Module.hpp"
#include "Oscillator.hpp"
#include "Output.hpp"

using namespace std;

int open_window()
{
  // Create an application window with the following settings:
  window = SDL_CreateWindow(
      "An SDL2 window",                  // window title
      SDL_WINDOWPOS_UNDEFINED,           // initial x position
      SDL_WINDOWPOS_UNDEFINED,           // initial y position
      640,                               // width, in pixels
      480,                               // height, in pixels
      SDL_WINDOW_OPENGL                  // flags - see below
  );

  // Check that the window was successfully created
  if (window == NULL) {
      // In the case that the window could not be made...
      cout << "Could not create window: " << SDL_GetError() << endl;
      return 0;
  }

  return 1;
}

// int create_renderer()
// {
//   renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
// }