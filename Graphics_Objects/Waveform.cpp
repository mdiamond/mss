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

/*****************************
 * WAVEFORM MEMBER FUNCTIONS *
 *****************************/

/*
 * Constructor.
 */
Waveform::Waveform(std::string _name, SDL_Rect _location,
                   SDL_Color *_color, SDL_Color *_background_color, float _range_low,
                   float _range_high, std::vector<float> *_buffer) :
    Graphics_Object(_name, WAVEFORM, NULL, _location, _color), 
    background_color(_background_color), range_low(_range_low),
    range_high(_range_high), buffer(_buffer)
{
    render_buffer = std::vector<float>(location.w, 0);
}

/*
 * Destructor.
 */
Waveform::~Waveform()
{

}

/*
 * Calculate the y pixel location in the waveform line at
 * the given sample i.
 */
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

/*
 * Copy the buffer to be rendered to inside of this waveform object.
 * This should always be done with audio locked before rendering to
 * avoid interleaving between the main thread and the audio thread.
 */
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
    std::vector<SDL_Point> points, points2, points3;

    points = std::vector<SDL_Point>(location.w, zero);

    for(unsigned int i = 0; i < render_buffer.size(); i ++)
    {
        points[i].x = location.x + i;
        points[i].y = calculate_y(i);
    }

    points2 = points;
    points3 = points;

    for(unsigned int i = 0; i < points.size(); i ++)
    {
        points2[i].y += 1;
        points3[i].y -= 1;
    }

    if(!SELECTING_SRC || (SELECTING_SRC && parent != NULL && parent->graphics_objects[0]->was_clicked()))
        SDL_SetRenderDrawColor(RENDERER, background_color->r, background_color->g, background_color->b, background_color->a);
    else
        SDL_SetRenderDrawColor(RENDERER, background_color->r, background_color->g, background_color->b, background_color->a / 2);
    SDL_RenderFillRect(RENDERER, &location);

    if(!SELECTING_SRC || (SELECTING_SRC && parent != NULL && parent->graphics_objects[0]->was_clicked()))
        SDL_SetRenderDrawColor(RENDERER, color->r, color->g, color->b, color->a);
    else
        SDL_SetRenderDrawColor(RENDERER, color->r, color->g, color->b, color->a / 2);
    SDL_RenderDrawLines(RENDERER, &points[0], points.size());
    SDL_RenderDrawLines(RENDERER, &points2[0], points.size());
    SDL_RenderDrawLines(RENDERER, &points3[0], points.size());
}

/*
 * Waveforms do not respond to clicks.
 */
void Waveform::clicked()
{

}
