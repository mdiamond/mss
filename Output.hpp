/*
 * Matthew Diamond 2015
 * The output module. All audio requested by the callback
 * function comes through here.
 */

#ifndef synth_output_h
#define synth_output_h

// Included libraries
#include <string>
#include <vector>

// Included classes
#include "Module.hpp"

class Output: public Module
{
  public:
    std::string name;
    std::vector<float> *input_l;
    std::vector<float> *input_r;
    Output(void);
    virtual ~Output(void);
    virtual void process(int);
};

#endif