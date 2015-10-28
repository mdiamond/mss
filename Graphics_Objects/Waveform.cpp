/*
 * Matthew Diamond 2015
 * Member functions for the Waveform class.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
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
Waveform::Waveform(string _name, SDL_Rect _location,
                   SDL_Color _color, float _range_low,
                   float _range_high, vector<float> *_buffer) :
    Graphics_Object(_name, WAVEFORM, NULL, _location, _color), 
    range_low(_range_low), range_high(_range_high), buffer(_buffer)
{
    render_buffer = vector<float>(location.w, 0);
}

/*
 * Destructor.
 */
Waveform::~Waveform()
{

}

float Waveform::calculate_y(int i)
{
    float sample;
    int y;

    if(range_low != -1 || range_high != 1)
        sample = scale_sample(render_buffer[i],
                              range_low, range_high, -1, 1);
    else
        sample = render_buffer[i];

    y = (location.y + location.h / 2) +
        (sample * -1) * (location.h / 2);

    if(y < location.y)
        y = location.y;
    if(y > location.y + location.h)
        y = location.y + location.h;

    return y;
}

void Waveform::copy_buffer()
{
    int index = 0;
    for(int i = BUFFER_SIZE - location.w; i < BUFFER_SIZE; i ++)
    {
        if(buffer != NULL)
            render_buffer[index] = (*buffer)[i];
        else
            render_buffer[index] = 0;
        index ++;
    }
}

/*
 * Render the waveform.
 */
void Waveform::render()
{
    SDL_Point zero = {0, 0};
    vector<SDL_Point> points(location.w, zero);

    for(int i = 0; i < render_buffer.size(); i ++)
    {
        points[i].x = location.x + i;
        points[i].y = calculate_y(i);
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
