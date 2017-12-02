/*
 * Matthew Diamond 2015
 * Image processing functions and graphics related
 * initialization functions
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <vector>

// Included files
#include "graphics_config.hpp"
#include "image_processing.hpp"
#include "main.hpp"
#include "main_helpers.hpp"
#include "signal_processing.hpp"

// Included modules classes
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"

// Included graphics classes
#include "Graphics_Objects/Button.hpp"
#include "Graphics_Objects/Page.hpp"
#include "Graphics_Objects/Rect.hpp"
#include "Graphics_Objects/Waveform.hpp"

/******************************
 * IMAGE PROCESSING FUNCTIONS *
 ******************************/

/*
 * Determine where in the window each of the modules and their
 * respective graphics objects are meant to be rendered.
 */
void update_graphics_objects()
{
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != NULL)
        {
            MODULES[i]->update_graphics_object_locations();
        }
    }

    MODULES_CHANGED = false;
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
    // (no top-level page has any graphics objects, only their sub pages
    // contain graphics objects)
    std::vector<Graphics_Object *> graphics_objects;
    std::vector<Graphics_Object *> sub_page_graphics_objects;
    std::string current_page_name;
    Page *current_page = NULL;

    // For each module
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(graphics_objects.size() == 0)
        {
            graphics_objects.push_back((Graphics_Object *) UTILITIES_PAGE);
        }

        if(MODULES[i] != NULL)
        {
            // Mark each graphics objects in the module as updated
            for(auto itr = MODULES[i]->graphics_objects.begin();
                itr != MODULES[i]->graphics_objects.end(); itr ++)
            {
                itr->second->updated = true;
            }

            // Add the module to the list of sub page graphics objects
            graphics_objects.push_back((Graphics_Object *) MODULES[i]);
        }

        // If this is the last sub page in the current page, or
        // if there are no more modules to take into consideration
        if(i % (MODULES_PER_COLUMN * MODULES_PER_ROW) ==
           (unsigned)(MODULES_PER_COLUMN * MODULES_PER_ROW) - 1
           || i == MODULES.size() - 1)
        {
            // Create the page using the created vector of sub pages, add it
            // to the global list of pages
            current_page = new Page(std::to_string(i / (MODULES_PER_COLUMN *
                                                        MODULES_PER_ROW)) + " (page)",
                                    WINDOW_RECT, BLACK, &graphics_objects);
            PAGES.push_back(current_page);
            current_page = NULL;
            graphics_objects.clear();
            sub_page_graphics_objects.clear();
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
        update_graphics_objects();
        calculate_pages();
    }

    // Clear the renderer
    SDL_RenderClear(RENDERER);

    // Copy audio data into waveform objects so that they will render without hiccups
    /* SDL_LockAudio(); */
    for(unsigned int i = 0; i < MODULES.size(); i ++)
    {
        if(MODULES[i] != NULL)
        {
            for(auto itr = MODULES[i]->graphics_objects.begin();
                itr != MODULES[i]->graphics_objects.end(); itr ++)
            {
                if(((Graphics_Object *) itr->second)->graphics_object_type
                   == Graphics_Object::WAVEFORM)
                {
                    copy_signal_to_waveform_buffer(((Waveform &) (*itr->second)));
                }
            }
        }
    }
    /* SDL_UnlockAudio(); */

    // Render graphics objects for the current page
    PAGES[CURRENT_PAGE]->render();

    // Copy the contents of the texture into the renderer, then present the renderer
    SDL_SetRenderTarget(RENDERER, NULL);
    SDL_RenderCopy(RENDERER, TEXTURE, NULL, NULL);
    SDL_RenderPresent(RENDERER);
    SDL_SetRenderTarget(RENDERER, TEXTURE);
}

