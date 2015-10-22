/*
 * Matthew Diamond 2015
 * Member functions for the Waveform class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "../main.hpp"
#include "../signal_processing.hpp"

// Included classes
#include "../Graphics_Object.hpp"
#include "Waveform.hpp"

using namespace std;

/*****************************
 * WAVEFORM MEMBER FUNCTIONS *
 *****************************/

/*
 * Constructor.
 */
Waveform::Waveform(string _name, SDL_Rect *_location,
                   SDL_Color *_color, float _range_low,
                   float _range_high, vector<float> *_buffer)
{
    name = _name;
    type = WAVEFORM;
    location = *_location;
    color = *_color;

    range_low = _range_low;
    range_high = _range_high;
    buffer = _buffer;
}

/*
 * Destructor.
 */
Waveform::~Waveform()
{

}

float Waveform::calculate_y(int i, int index)
{
    float sample;

    if(buffer != NULL)
        sample = scale_sample(((*(buffer))[BUFFER_SIZE - location.w + index]),
                              range_low, range_high, -1, 1);
    else
        sample = 0;

    int y;
    if(buffer != NULL)
        y = (location.y + location.h / 2) +
            (sample * -1) * (location.h / 2);
    else
        y = (location.y + location.h / 2) +
            0 *
            (location.h / 2);
    if(y < location.y)
        y = location.y;
    if(y > location.y + location.h)
        y = location.y + location.h;
    return y;
}

/*
 * Render the waveform.
 */
void Waveform::render()
{
    SDL_Point zero = {0, 0};
    vector<SDL_Point> points(location.w, zero);

    int index = 0;
    for(int i = BUFFER_SIZE - location.w; i < BUFFER_SIZE; i ++)
    {
        points[index].x = location.x + index;
        points[index].y = calculate_y(i, index);
        index ++;
    }

    SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
    SDL_RenderFillRect(RENDERER, &location);

    SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255);
    SDL_RenderDrawLines(RENDERER, &points[0], points.size());
}

/*
 * Dummy function.
 */
void Waveform::clicked()
{

}
