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
Waveform::Waveform(string *_name, SDL_Rect *_location, SDL_Color *_color, vector<float> *_buffer)
{
    name = *_name;
    type = WAVEFORM;
    location = *_location;
    color = *_color;
    buffer = _buffer;
}

/*
 * Dummy function.
 */
Waveform::~Waveform()
{

}

/*
 * Render the waveform.
 */
void Waveform::render_graphics_object()
{
    SDL_Point zero = {0, 0};
    vector<SDL_Point> points(location.w, zero);

    int index = 0;
    for(unsigned int i = buffer->size() - location.w; i < buffer->size(); i ++)
    {
        points[index].x = location.x + index;
        points[index].y = (location.y + location.h / 2) +
                          ((*(buffer))[buffer->size() - location.w + index]) * (location.h / 2);
        index ++;
    }

    SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 255);
    SDL_RenderFillRect(RENDERER, &location);

    SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255);
    SDL_RenderDrawLines(RENDERER, &points[0], points.size());
}
