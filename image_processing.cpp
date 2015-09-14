/*
 * Matthew Diamond 2015
 * This file contains all of the basic image processing functions.
 * It also contains helper functions for main().
 */

// Included files
#include "image_processing.hpp"

using namespace std;

SDL_Point zero = {0, 0};
vector<SDL_Point> samples(WINDOW_WIDTH, zero);

/****************************
 * INITIALIZATION FUNCTIONS *
 ****************************/

/*
 * Open a window using width and height specified in main.cpp.
 */
int open_window()
{
  WINDOW = SDL_CreateWindow(
      "Visualizer",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_OPENGL
  );

  if (WINDOW == NULL) {
      cout << "Could not create window: " << SDL_GetError() << endl;
      return 0;
  }

  return 1;
}

/*
 * Create a renderer for the window created before.
 */
int create_renderer()
{
  RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);

  if (WINDOW == NULL) {
    cout << "Could not create renderer: " << SDL_GetError() << endl;
    return 0;
  }

  return 1;
}

/******************************
 * IMAGE PROCESSING FUNCTIONS *
 ******************************/

/*
 * Take a two buffers of sound, average them,
 * and calculate the points necessary to render
 * them as waveforms. Store these points as SDL_Point
 * structs in the samples vector.
 */
void populate_samples(vector<float> *buffer_l, vector<float> *buffer_r)
{
  // First, shift over all samples in the vector to the left by BUFFER_SIZE
  for(unsigned int i = 0; i + BUFFER_SIZE < samples.size(); i ++)
  {
    samples[i].x = i;
    samples[i].y = samples[i + BUFFER_SIZE].y;
  }
  // Next, calculate the average of the left and right channel amplitudes
  // for each sample, put them in the samples vector as y coordinates along
  // with their indices in the vector as x coordinates as SDL_Point structs.
  int middle = WINDOW_HEIGHT / 2;
  int index_samples = WINDOW_WIDTH - 1;
  int index_buffer = BUFFER_SIZE - 1;
  while(index_samples >= 0 && index_buffer >= 0)
  {
    int y = (int) middle + (middle * (((*buffer_l)[index_buffer] + (*buffer_r)[index_buffer]) / 2));
    samples[index_samples].x = index_samples;
    samples[index_samples].y = y;
    index_samples --;
    index_buffer --;
  }
}

/*
 * Construct the waveform in the window using a renderer
 * to draw lines between the points calculated previously.
 */
void draw_surface()
{
  // Fill the screen surface with black
  SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
  SDL_RenderClear(RENDERER);

  SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255);
  SDL_RenderDrawLines(RENDERER, &samples[0], samples.size());

  SDL_RenderPresent(RENDERER);
}
