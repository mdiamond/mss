/*
 * Matthew Diamond 2015
 * This file contains all of the basic signal processing functions.
 * It also contains helper functions for the callback function
 * that are utilized to fill the audio buffer.
 */

// Included files
#include "image_processing.hpp"

using namespace std;

SDL_Point zero = {0, 0};
vector<SDL_Point> samples(WINDOW_WIDTH, zero);

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

void populate_samples(vector<float> *buffer_l, vector<float> *buffer_r)
{
  for(int i = 0; i + BUFFER_SIZE < samples.size(); i ++)
  {
    samples[i] = samples[i + BUFFER_SIZE];
  }
  int middle = WINDOW_HEIGHT / 2;
  int index_samples = WINDOW_WIDTH - 1;
  int index_buffer = BUFFER_SIZE - 1;
  while(index_samples >= 0 && index_buffer >= 0)
  {
    int y = (int) middle + (middle * (((*buffer_l)[index_buffer] + (*buffer_r)[index_buffer]) / 2));
    SDL_Point point = {index_samples, y};
    samples[index_samples] = point;
    index_samples --;
    index_buffer --;
  }
}

void draw_surface()
{
  // Fill the screen surface with black
  SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
  SDL_RenderClear(RENDERER);

  // for (int i = 0; i < WINDOW_WIDTH; ++i)
  // {
  //   cout << samples[i].x << "," << samples[i].y << endl;
  // }

  SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255);
  SDL_RenderDrawLines(RENDERER, &samples[0], samples.size());

  SDL_RenderPresent(RENDERER);
}