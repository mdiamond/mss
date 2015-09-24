/*
 * Matthew Diamond 2015
 * The Page graphics object. This object is a page
 * full of modules or a special page for things like
 * saving, adding a module, etc.
 */

#ifndef synth_page_h
#define synth_page_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

/*************************
 * PAGE CLASS DEFINITION *
 *************************/

class Page: public Graphics_Object
{
    public:
        std::vector<Graphics_Object *> *graphics_objects;
        // Constructor and destructor
        Page(SDL_Rect *, SDL_Color *, std::vector<Graphics_Object *> *);
        virtual ~Page();
        // Member functions
        virtual void render_graphics_object();
        virtual void clicked();
};

#endif
