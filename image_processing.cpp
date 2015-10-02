/*
 * Matthew Diamond 2015
 * This file contains all of the basic image processing functions.
 * It also contains helper functions for main().
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <queue>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "image_processing.hpp"
#include "main.hpp"
#include "signal_processing.hpp"

// Included classes
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Page.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Module.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"

using namespace std;

/****************************
 * INITIALIZATION FUNCTIONS *
 ****************************/

/*
 * Open a window using width and height specified in main.cpp.
 */
int open_window()
{
    WINDOW = SDL_CreateWindow("Visualizer",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_OPENGL
                             );

    if (WINDOW == NULL) {
        cout << "Could not create window: " << SDL_GetError() << endl;
        return 0;
    }

    cout << "Window opened." << endl;

    return 1;
}

/*
 * Create a renderer for the window created before.
 */
int create_renderer()
{
    RENDERER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED);

    if (WINDOW == NULL) {
        cout << "Could not create renderer: " << SDL_GetError() << endl;
        return 0;
    }

    cout << "Renderer created." << endl;

    return 1;
}

/*
 * Load fonts into external variables so that all
 * modules can render text.
 */
int load_fonts()
{
    FONT_REGULAR = TTF_OpenFont("fonts/SourceCodePro-Regular.ttf", 11);
    FONT_BOLD = TTF_OpenFont("fonts/SourceCodePro-Bold.ttf", 11);

    if(!FONT_REGULAR || !FONT_BOLD)
    {
        cout << "Could not open one of the TTF fonts: " << TTF_GetError() << endl;
        return 0;
    }

    cout << "Fonts loaded." << endl;

    return 1;
}

/******************************
 * IMAGE PROCESSING FUNCTIONS *
 ******************************/

/*
 * Determine where in the window each of the modules and their
 * respective graphics objects are meant to be rendered.
 */
void calculate_graphics_objects()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        MODULES[i]->calculate_graphics_objects();
    }
    MODULES_CHANGED = 0;
}

bool render_special_pages()
{
    return false;

    // Variables for the current sub page, its sub pages,
    // its sub pages graphics objects, and the current page
    // (no page has any graphics objects, only sub pages
    // which contain graphics objects)
    string current_sub_page_name;
    Page *current_sub_page;
    vector<Page> *sub_pages = new vector<Page>();
    vector<Graphics_Object *> *sub_page_graphics_objects = new vector<Graphics_Object *>();
    string current_page_name;
    Page *current_page;

    // Create the background and add it to the list of graphics
    // objects
    string object_name = "background (rect)";
    Rect *background = new Rect(&object_name, &WINDOW_RECT, &BLACK);
    sub_page_graphics_objects->push_back(background);

    // Create the sub page and add it to the list of sub pages
    // for the current page
    current_sub_page_name = "utilities & background (page)";
    current_sub_page = new Page(&current_sub_page_name, &WINDOW_RECT, &BLACK,
                                      sub_page_graphics_objects, NULL);
    sub_pages->push_back(*current_sub_page);
    sub_page_graphics_objects = new vector<Graphics_Object *>();

    return true;
}

void initialize_utilities_sub_page(vector<Graphics_Object *> *sub_page_graphics_objects,
                                   vector<Page> *sub_pages, Page *current_sub_page)
{
    int x = 2;
    // Create the background and add it to the list of graphics
    // objects
    string object_name = "background (rect)";
    Rect *background = new Rect(&object_name, &WINDOW_RECT, &BLACK);
    sub_page_graphics_objects->push_back(background);

    // Create the "add module" button and add it to the
    // list of graphics objects
    SDL_Rect location = {x , WINDOW_HEIGHT - 17, 100, 15};
    x += 100;
    object_name = "add oscillator (button)";
    string button_text = "ADD OSCILLATOR";
    Module *parent = NULL;
    Button *button = new Button(&object_name, &location, &WHITE,
                                &button_text, parent);
    sub_page_graphics_objects->push_back(button);

    // Create the "previous page" button and add it to the
    // list of graphics objects
    x += 2;
    location = {x, WINDOW_HEIGHT - 17, 92, 15};
    x += 92;
    object_name = "previous page (button)";
    button_text = "PREVIOUS PAGE";
    parent = NULL;
    button = new Button(&object_name, &location, &WHITE,
                        &button_text, parent);
    sub_page_graphics_objects->push_back(button);

    // Create the "next page" button and add it to the
    // list of graphics objects
    x += 2;
    location = {x, WINDOW_HEIGHT - 17, 65, 15};
    x += 65;
    object_name = "next page (button)";
    button_text = "NEXT PAGE";
    parent = NULL;
    button = new Button(&object_name, &location, &WHITE,
                        &button_text, parent);
    sub_page_graphics_objects->push_back(button);

    // Create the sub page and add it to the list of sub pages
    // for the current page
    string current_sub_page_name = "utilities & background (page)";
    current_sub_page = new Page(&current_sub_page_name, &WINDOW_RECT, &BLACK,
                                      sub_page_graphics_objects, NULL);
    sub_pages->push_back(*current_sub_page);
    sub_page_graphics_objects = new vector<Graphics_Object *>();
}

/*
 * Determine what graphics objects go on what pages. Create those pages.
 */
void calculate_pages()
{
    // if(render_special_pages())
    //     return;

    // Variables for the current sub page, its sub pages,
    // its sub pages graphics objects, and the current page
    // (no page has any graphics objects, only sub pages
    // which contain graphics objects)
    string current_sub_page_name;
    Page *current_sub_page;
    vector<Page> *sub_pages = new vector<Page>();
    vector<Graphics_Object *> *sub_page_graphics_objects = new vector<Graphics_Object *>();
    string current_page_name;
    Page *current_page;

    // Create the first sub page, which will contain important
    // buttons for special functions
    initialize_utilities_sub_page(sub_page_graphics_objects, sub_pages, current_sub_page);

    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        current_sub_page_name = MODULES[i]->name + " (page)";
        for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
        {
            sub_page_graphics_objects->push_back(MODULES[i]->graphics_objects[j]);
        }
        current_sub_page = new Page(&current_sub_page_name,
                                    &MODULES[i]->graphics_objects[1]->location, &BLACK,
                                    sub_page_graphics_objects, NULL);
        sub_pages->push_back(*current_sub_page);

        sub_page_graphics_objects = new vector<Graphics_Object *>();

        if(i == (unsigned) ((MODULES_PER_COLUMN * MODULES_PER_ROW) - 1) ||
           i == MODULES.size() - 1)
        {
            current_page_name = to_string(i / (MODULES_PER_COLUMN * MODULES_PER_ROW)) + " (page)";
            current_page = new Page(&current_page_name, &WINDOW_RECT, &BLACK,
                                    NULL, sub_pages);
            if(i / (MODULES_PER_COLUMN * MODULES_PER_ROW) < PAGES.size())
                PAGES[i / (MODULES_PER_COLUMN * MODULES_PER_ROW)] = *current_page;
            else
                PAGES.push_back(*current_page);

            sub_pages = new vector<Page>();
            sub_page_graphics_objects = new vector<Graphics_Object *>();
            initialize_utilities_sub_page(sub_page_graphics_objects, sub_pages, current_sub_page);
        }
    }
}

/*
 * Render the GUI in the window by using the
 * member functions in each module that render
 * their representations
 */
void draw_surface()
{
    // First, lock the audio
    SDL_LockAudio();

    // Once audio is locked, copy all data necessary for graphics
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        MODULES[i]->copy_graphics_data();
    }

    // Once all data necessary for graphics has been copied,
    // unlock the audio
    SDL_UnlockAudio();

    // If the set of modules has changed, calculate the
    // graphics objects, then calculate the pages
    if(MODULES_CHANGED)
    {
        calculate_graphics_objects();
        calculate_pages();
        MODULES_CHANGED = false;
    }

    // If the current page exists, render it, otherwise,
    // render a black rectangle over the whole window
    if((unsigned int) CURRENT_PAGE < PAGES.size())
        PAGES[CURRENT_PAGE].render_graphics_object();
    else
    {
        SDL_SetRenderDrawColor(RENDERER, BLACK.r, BLACK.g, BLACK.b, BLACK.a);
        SDL_Rect location = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
        SDL_RenderFillRect(RENDERER, &location);
    }

    // Present what has been rendered
    SDL_RenderPresent(RENDERER);
}
