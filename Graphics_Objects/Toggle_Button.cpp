/*
 * Matthew Diamond 2015
 * Member functions for the Toggle Button class.
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
#include "Toggle_Button.hpp"

using namespace std;

/**********************************
 * TOGGLE BUTTON MEMBER FUNCTIONS *
 **********************************/

/*
 * Constructor.
 */
Toggle_Button::Toggle_Button(string *_name, SDL_Rect *_location, SDL_Color *_color, bool *_b)
{
    name = *_name;
    type = TOGGLE_BUTTON;
    location = *_location;
    color = *_color;
    b = _b;
}

/*
 * Dummy function.
 */
Toggle_Button::~Toggle_Button()
{

}

/*
 * Render the Toggle_Button.
 */
void Toggle_Button::render_graphics_object()
{


}

void Toggle_Button::toggle()
{
    *b = !(*b);
}
