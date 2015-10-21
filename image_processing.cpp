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
#include "main_helpers.hpp"
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
    WINDOW = SDL_CreateWindow("synth",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH,
                              WINDOW_HEIGHT,
                              SDL_WINDOW_OPENGL);

    if (WINDOW == NULL) {
        cout << RED_STDOUT << "Could not create window: "
             << SDL_GetError() << DEFAULT_STDOUT << endl;
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
        cout << RED_STDOUT << "Could not create renderer: "
             << SDL_GetError() << DEFAULT_STDOUT << endl;
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
    FONT_SMALL = TTF_OpenFont("fonts/SourceCodePro-Regular.ttf", 10);
    FONT_BOLD = TTF_OpenFont("fonts/SourceCodePro-Bold.ttf", 11);

    if(!FONT_REGULAR || !FONT_BOLD)
    {
        cout << RED_STDOUT << "Could not open one of the TTF fonts: "
             << TTF_GetError() << DEFAULT_STDOUT << endl;
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

Module *hovering_over()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(!MODULES[i]->graphics_objects[0]->was_clicked())
            return MODULES[i];

    return NULL;
}

void select_input_mode()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        if(!MODULES[i]->graphics_objects[0]->was_clicked())
            for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
                MODULES[i]->graphics_objects[j]->color.a = 50;
        else
            for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
                MODULES[i]->graphics_objects[j]->color.a = 255;
}

void reset_alphas()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
        for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
            MODULES[i]->graphics_objects[j]->color.a = 255;
}

/*
 * Create the very first sub page on every page, the utilities sub page.
 * This sub page contains the buttons for adding new modules, switching
 * pages, and saving or loading the current patch.
 */
void initialize_utilities_sub_page(vector<Graphics_Object *> *sub_page_graphics_objects,
                                   vector<Page *> *sub_pages, Page *current_sub_page)
{
    int x = 2;
    Module *parent = NULL;

    // Create the background and add it to the list of graphics
    // objects
    Rect *background = new Rect("background (rect)", &WINDOW_RECT, &BLACK, NULL);
    sub_page_graphics_objects->push_back(background);

    // Create the "add module" button and add it to the
    // list of graphics objects
    SDL_Rect location = {x , WINDOW_HEIGHT - 17, 100, 15};
    x += 102;
    Button *button = new Button("add oscillator (button)", &location, &WHITE,
                                "ADD OSCILLATOR", parent);
    sub_page_graphics_objects->push_back(button);

    // Create the "add VCA" button and add it to the
    // list of graphics objects
    location = {x, WINDOW_HEIGHT - 17, 51, 15};
    x += 53;
    button = new Button("add vca (button)", &location, &WHITE,
                                "ADD VCA", parent);
    sub_page_graphics_objects->push_back(button);

    // Create the "add mixer" button and add it to the
    // list of graphics objects
    location = {x, WINDOW_HEIGHT - 17, 51, 15};
    button = new Button("add mixer (button)", &location, &WHITE,
                                "ADD MIXER", parent);
    sub_page_graphics_objects->push_back(button);

    // Create the "previous page" button and add it to the
    // list of graphics objects
    x = WINDOW_WIDTH - 159;
    location = {x, WINDOW_HEIGHT - 17, 92, 15};
    x += 92;
    button = new Button("previous page (button)", &location, &WHITE,
                        "PREVIOUS PAGE", parent);
    sub_page_graphics_objects->push_back(button);

    // Create the "next page" button and add it to the
    // list of graphics objects
    x += 2;
    location = {x, WINDOW_HEIGHT - 17, 63, 15};
    x += 65;
    button = new Button("next page (button)", &location, &WHITE,
                        "NEXT PAGE", parent);
    sub_page_graphics_objects->push_back(button);

    // Create the sub page and add it to the list of sub pages
    // for the current page
    current_sub_page = new Page("utilities & background (page)", &WINDOW_RECT, &BLACK,
                                      sub_page_graphics_objects, NULL);
    sub_pages->push_back(current_sub_page);
}

/*
 * Determine what graphics objects go on what pages. Create those pages.
 */
void calculate_pages()
{
    destroy_pages();
    PAGES.clear();

    // Variables for the current sub page, its sub pages,
    // its sub pages graphics objects, and the current page
    // (no page has any graphics objects, only sub pages
    // contain graphics objects)
    string current_sub_page_name;
    Page *current_sub_page = NULL;
    vector<Page *> *sub_pages = new vector<Page *>();
    vector<Graphics_Object *> *sub_page_graphics_objects = new vector<Graphics_Object *>();
    string current_page_name;
    Page *current_page = NULL;

    // Create the first sub page, which will contain important
    // buttons for special functions
    initialize_utilities_sub_page(sub_page_graphics_objects, sub_pages, current_sub_page);

    // Reset the sub page graphics objects
    delete sub_page_graphics_objects;
    sub_page_graphics_objects = new vector<Graphics_Object *>();

    // For each module
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        // Add each of its graphics objects to the current sub page
        for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
        {
            sub_page_graphics_objects->push_back(MODULES[i]->graphics_objects[j]);
            MODULES[i]->graphics_objects[j]->updated = true;
        }

        // Create the sub page using the created vector of graphics objects,
        // add it to the list of sub pages
        current_sub_page = new Page(MODULES[i]->name + " (page)",
                                    &MODULES[i]->graphics_objects[1]->location, &BLACK,
                                    sub_page_graphics_objects, NULL);
        sub_pages->push_back(current_sub_page);

        // Reset the sub page graphics objects
        delete sub_page_graphics_objects;
        sub_page_graphics_objects = new vector<Graphics_Object *>();

        // If this is the last sub page in the current page, or
        // if there are no more modules to take into consideration
        if(i % (MODULES_PER_COLUMN * MODULES_PER_ROW) ==
           (unsigned) (MODULES_PER_COLUMN * MODULES_PER_ROW) - 1 ||
           i == MODULES.size() - 1)
        {
            // Create the page using the created vector of sub pages, add it
            // to the global list of pages
            current_page = new Page(to_string(i / (MODULES_PER_COLUMN * MODULES_PER_ROW)) + " (page)",
                                    &WINDOW_RECT, &BLACK,
                                    NULL, sub_pages);
            PAGES.push_back(current_page);

            // Delete the vector of sub pages and sub page graphics objects,
            // only re-initialize them and restart the process of calculating a new page
            // if there are still modules left to take into consideration
            delete sub_pages;
            delete sub_page_graphics_objects;
            if(i != MODULES.size() - 1)
            {
                sub_pages = new vector<Page *>();
                sub_page_graphics_objects = new vector<Graphics_Object *>();
                initialize_utilities_sub_page(sub_page_graphics_objects, sub_pages, current_sub_page);
                delete sub_page_graphics_objects;
                sub_page_graphics_objects = new vector<Graphics_Object *>();
            }
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
    // If the set of modules has changed, calculate the
    // graphics objects, then calculate the pages
    if(MODULES_CHANGED)
    {
        calculate_graphics_objects();
        calculate_pages();
        MODULES_CHANGED = false;
    }

    // Clear the renderer
    SDL_RenderClear(RENDERER);

    // Update graphics objects for all modules
    for(unsigned int i = CURRENT_PAGE * MODULES_PER_PAGE;
        i < (CURRENT_PAGE + 1) * MODULES_PER_PAGE && i < MODULES.size();
        i ++)
        MODULES[i]->update_graphics_objects();

    if(SELECTING_SRC)
        select_input_mode();

    // Render graphics objects for the current page
    PAGES[CURRENT_PAGE]->render();

    // Present what has been rendered
    SDL_RenderPresent(RENDERER);
}
