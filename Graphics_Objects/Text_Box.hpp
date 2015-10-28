/*
 * Matthew Diamond 2015
 * The Text Box graphics object. This object allows the user
 * to type into it. The data entered can be used to change variables.
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
        bool active;
        TTF_Font *font;
        Rect background;
        Text text;
        Text prompt_text;
        Text typing_text;
        // Constructor and destructor
        Text_Box(std::string, SDL_Rect, SDL_Color,
                 std::string, std::string,
                 TTF_Font *, Module *);
        virtual ~Text_Box();
        // Member functions
        virtual void render();
        virtual void clicked();
        void add_characters(char *);
        void delete_character();
        void entered();
        void update_location(SDL_Rect);
        void update_current_text(std::string);
};

#endif
