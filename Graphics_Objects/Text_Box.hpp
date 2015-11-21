/*
 * Matthew Diamond 2015
 * The Text Box graphics object. This object allows the user
 * to type into it. The data entered can be used to change parameters.
 */

#ifndef synth_text_box_h
#define synth_text_box_h

/************
 * INCLUDES *
 ************/

// Included SDL componenets
#include "SDL2/SDL.h"

// Included classes
#include "Rect.hpp"
#include "Text.hpp"

/*****************************
 * TEXT BOX CLASS DEFINITION *
 *****************************/

class Text_Box: public Graphics_Object
{
    public:
        // The text color, whether or not this text box
        // is active, the font, the background rectangle,
        // the text, the prompt text, and the typing text
        SDL_Color text_color;
        bool active;
        TTF_Font *font;
        Rect background;
        Text text;
        Text prompt_text;
        Text typing_text;

        // Constructor and destructor
        Text_Box(std::string, SDL_Rect, SDL_Color, SDL_Color,
                 std::string, TTF_Font *, Module *);
        virtual ~Text_Box();

        // Virtual member functions
        virtual void render();
        virtual void clicked();

        // Member functions
        //   Add character to the typing buffer
        void add_characters(char *);
        //   Delete the final character from the typing buffer
        void delete_character();
        //   Confirm the entry in the text box
        void entered();
        //   Override the default graphics object update_location() function
        void update_location(SDL_Rect);
        //   Update the currently displayed text
        void update_current_text(std::string);
};

#endif
