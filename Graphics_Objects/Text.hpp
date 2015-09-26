/*
 * Matthew Diamond 2015
 * The Text graphics object. This object is just some text
 * in some place in the window, with some color.
 */

#ifndef synth_text_h
#define synth_text_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

/*************************
 * TEXT CLASS DEFINITION *
 *************************/

class Text: public Graphics_Object
{
    public:
        TTF_Font *font;
        std::string original_text;
        std::string current_text;
        std::string old_text;
        std::string *live_text;
        SDL_Texture *texture;
        // Constructor and destructor
        Text(std::string *, SDL_Rect *, SDL_Color *,
             std::string *, std::string *, TTF_Font *);
        virtual ~Text();
        // Member functions
        virtual void render_graphics_object();
        virtual void clicked();
};

#endif
