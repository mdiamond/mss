/*
 * Matthew Diamond 2015
 * Member functions for the Output class
 */

// Included libraries
#include <string>

// Included classes
#include "Module.hpp"
#include "Output.hpp"

using namespace std;
  
Output::Output(void)
{
  name = "output";
  type = OUTPUT;
  input_l = NULL;
  input_r = NULL;
}

Output::~Output(void)
{

}

void Output::process(int num_samples)
{
  process_depends(num_samples);
}