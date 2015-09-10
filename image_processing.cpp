/*
 * Matthew Diamond 2015
 * This file contains all of the basic signal processing functions.
 * It also contains helper functions for the callback function
 * that are utilized to fill the audio buffer.
 */

// Included libraries
#include <iostream>
#include <queue>
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

vector<float> samples(WINDOW_WIDTH, 0);

int open_window()
{
  // Create an application window with the following settings:
  WINDOW = SDL_CreateWindow(
      "Visualizer",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_OPENGL
  );

  // Check that the window was successfully created
  if (WINDOW == NULL) {
      // In the case that the window could not be made...
      cout << "Could not create window: " << SDL_GetError() << endl;
      return 0;
  }

  return 1;
}

void put_white_pixel(SDL_Surface *screen, int x, int y)
{
    Uint8 *pixel = (Uint8*) screen->pixels;
    pixel += (y * screen->pitch) + (x * sizeof(Uint32));
    *((Uint32*)pixel) = 0xFFFFFFFF;
}

void fill_surface_waveform(SDL_Surface *screen, vector<float> *buffer)
{
  Uint32 *pixels = (Uint32 *) screen->pixels;

  for(int x = WINDOW_WIDTH; x > 0; x --)
  {
    int y = (int) (WINDOW_HEIGHT / 2) + ((WINDOW_HEIGHT / 2) * samples[WINDOW_WIDTH - x]);
    put_white_pixel(screen, x, y);
  }
}

void populate_samples(vector<float> *buffer)
{
  for(int i = 0; i + BUFFER_SIZE < samples.size(); i ++)
  {
    samples[i] = samples[i + BUFFER_SIZE];
  }
  int index = WINDOW_WIDTH - BUFFER_SIZE - 1;
  for(int i = 0; i < BUFFER_SIZE && index >= 0; i ++)
  {
    samples[index] = (*buffer)[i];
    index ++;
  }
}

void update_surface(vector<float> *buffer)
{
  // Get the screen surface
  SDL_Surface *screen = SDL_GetWindowSurface(WINDOW);

  // Fill the screen surface with black
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0x00000000, 0x00000000, 0x00000000));

  // Draw in the most recent waveform samples
  fill_surface_waveform(screen, buffer);
}