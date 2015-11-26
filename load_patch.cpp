/*
 * Matthew Diamond 2015
 * Functions for loading a patch.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

// Included files
#include "function_forwarder.hpp"
#include "main.hpp"

// Included classes
#include "Module.hpp"
#include "Modules/Mixer.hpp"
#include "Modules/Oscillator.hpp"
#include "Modules/Output.hpp"
#include "Modules/Multiplier.hpp"

/**************
 * LOAD PATCH *
 **************/

void load_patch(std::string filename)
{
    std::ifstream infile;

    infile.open(filename);

    if(infile.is_open())
    {
        std::string line;
        Module *module;
        std::vector<std::vector<std::string> > dependency_vectors;
        bool module_created, input_floats_copied, dependencies_copied;
        std::vector<std::string> dependencies;
        unsigned int dependency_num = 0;
        int oscillator_waveform_type;

        dependency_vectors = std::vector<std::vector<std::string> >();
        module_created = false;
        input_floats_copied = false;
        dependencies_copied = false;

        for(unsigned int i = 1; i < MODULES.size(); i ++)
        {
            delete MODULES[i];
            MODULES.erase(MODULES.begin() + i);
        }

        while(getline(infile, line))
        {
            if(line == "DONE")
            {
                // std::cout << "DONE LINE: " << line << std::endl;
                module_created = false;
                input_floats_copied = false;
                dependencies_copied = false;
                module->initialize_graphics_objects();
                if(module->type == OSCILLATOR)
                    ((Oscillator *) module)->switch_waveform(oscillator_waveform_type);
                module = NULL;
            }
            else if(!module_created)
            {
                // std::cout << "MODULE LINE: " << line << std::endl;
                switch(stoi(line))
                {
                    case MIXER: module = new Mixer(); break;
                    case MULTIPLIER: module = new Multiplier(); break;
                    case OSCILLATOR: module = new Oscillator(); break;
                }
                MODULES.push_back(module);
                module_created = true;
            }
            else if(!input_floats_copied)
            {
                // std::cout << "FLOAT LINE " << dependency_num << ": " << line << std::endl;
                module->set(stof(line), dependency_num);
                dependency_num ++;
                if(dependency_num == module->dependencies.size())
                {
                    input_floats_copied = true;
                    dependency_num = 0;
                }
            }
            else if(!dependencies_copied)
            {
                // std::cout << "DEPENDENCY LINE: " << dependency_num << ": " << line << std::endl;
                dependencies.push_back(line);
                dependency_num ++;
                if(dependency_num == module->dependencies.size())
                {
                    dependencies_copied = true;
                    dependency_num = 0;
                    dependency_vectors.push_back(dependencies);
                    dependencies = std::vector<std::string>();
                }
            }
            else
            {
                if(module->type == OSCILLATOR)
                {
                    // std::cout << "OSCILLATOR CURRENT PHASE LINE: " << line << std::endl;
                    ((Oscillator *) module)->current_phase = stof(line);
                    getline(infile, line);
                    // std::cout << "OSCILLATOR WAVEFORM TYPE LINE: " << line << std::endl;
                    oscillator_waveform_type = stoi(line);
                }
            }
        }

        // std::cout << "Finished reading in all modules" << std::endl;

        for(unsigned int i = 1; i < MODULES.size(); i ++)
        {
            for(unsigned int j = 0; j < MODULES[i]->dependencies.size(); j ++)
            {
                Module *src = find_module(&((dependency_vectors[i - 1])[j]), &MODULES);
                if(src != NULL)
                    MODULES[i]->set(src, j);
            }
        }

        // std::cout << "Finished setting module dependencies" << std::endl;

        // Make sure that the text boxes in all modules accurately represent their inputs,
        // then update the module name text object
        for(unsigned int i = 0; i < MODULES.size(); i ++)
        {
            int dependency_num = 0;
            std::string dependency_short_name;
            Input_Text_Box *input_text_box;
            Text *text;

            for(unsigned int j = 0; j < MODULES[i]->graphics_objects.size(); j ++)
            {
                if(MODULES[i]->graphics_objects[j]->type == INPUT_TEXT_BOX
                   && MODULES[i]->inputs_live[dependency_num])
                {
                    input_text_box = (Input_Text_Box *) MODULES[i]->graphics_objects[j];
                    dependency_short_name = MODULES[i]->dependencies[dependency_num]->get_short_name();
                    input_text_box->update_current_text(dependency_short_name);
                    dependency_num ++;
                }
                else if(MODULES[i]->graphics_objects[j]->type == INPUT_TEXT_BOX
                        && !MODULES[i]->inputs_live[dependency_num])
                {
                    input_text_box = (Input_Text_Box *) MODULES[i]->graphics_objects[j];
                    input_text_box->update_current_text(std::to_string(MODULES[i]->input_floats[dependency_num]));
                    dependency_num ++;
                }
                else if(MODULES[i]->graphics_objects[j]->type == INPUT_TEXT_BOX
                        && !MODULES[i]->inputs_live[dependency_num])
                    dependency_num ++;
            }

            text = (Text *) MODULES[i]->graphics_objects[MODULE_NAME_TEXT];
            text->update_text(MODULES[i]->name);
        }

        // std::cout << "Finished fixing module text boxes" << std::endl;

        infile.close();

        MODULES_CHANGED = true;

        std::cout << "Patch " << filename << " loaded" << std::endl;
    }
    else
        std::cout << RED_STDOUT << "Patch could not be loaded!" << DEFAULT_STDOUT << std::endl;
}