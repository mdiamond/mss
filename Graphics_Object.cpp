/*
 * Matthew Diamond 2015
 * Member functions for the Graphics_Object class.
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
#include "main.hpp"

// Included classes
#include "Graphics_Object.hpp"

using namespace std;

/************************************
 * GRAPHICS_OBJECT MEMBER FUNCTIONS *
 ************************************/

/*
 * Dummy function.
 */
Graphics_Object::Graphics_Object()
{

}

/*
 * Dummy function.
 */
Graphics_Object::~Graphics_Object()
{

}

/*
 * Check if this graphics object was clicked.
 * Return true if it was, false if it wasn't.
 */
bool Graphics_Object::was_clicked()
{
    // Certain types of graphics objects need not respond to clicks
    if(type == RECT || type == TEXT || type == WAVEFORM)
    {
        return false;
    }

    else if(MOUSE_X >= location.x && MOUSE_X < location.x + location.w &&
       MOUSE_Y >= location.y && MOUSE_Y < location.y + location.h)
    {
        return true;
    }

    return false;
}