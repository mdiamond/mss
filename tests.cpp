/*
 * Matthew Diamond 2015
 * Functions for testing other functions.
 */

/************
 * INCLUDES *
 ************/

// Included libraries
#include <iostream>
#include <string>
#include <vector>

// Included SDL components
#include "SDL2/SDL.h"

// Included files
#include "signal_processing.hpp"
#include "tests.hpp"

using namespace std;

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * Compare two buffers. Return true if they are the same,
 * false if they are not.
 */
bool compare_buffers(vector<float> *buffer1, vector<float> *buffer2, int num_samples)
{
    bool same = true;

    for(int i = 0; i < num_samples; i ++)
    {
        if((*buffer1)[i] != (*buffer2)[i])
            same = false;
    }

    return same;
}

/*
 * Print out the results of all tests.
 */
void print_test_results(string *names, int *results, int test_num)
{
    for(int i = 0; i < test_num; i ++)
    {
        cout << "Test #" << i + 1 << ": " << names[i] << " - ";
        if(results[i])
            cout << "PASSED!";
        else
            cout << "FAILED!"; 
        cout << endl;
    }
}

/*
 * Check if all tests have passed or not and inform the user.
 * Return 1 if all tests have passed, 0 if not.
 */
bool all_tests_passed(int *results, int test_num)
{
    bool passed = true;

    for(int i = 0; i < test_num ; i ++)
    {
        if(!results[i])
            passed = false;
    }

    if(passed)
        cout << "ALL TESTS PASSED!" << endl;
    else
        cout << "TESTS FAILED!" << endl;

    return passed;
}

/*********
 * TESTS *
 *********/

/*
 * Test #1 for the add_signals() function.
 */
bool test_add_signals_1()
{
    int num_samples = 4;
    vector<float> buffer1(num_samples, 0);
    vector<float> buffer2(num_samples, 0);
    vector<float> expected_buffer(num_samples, 0);
    vector<float> result_buffer(num_samples, 0);

    buffer1[0] = .5;
    buffer2[0] = .5;
    expected_buffer[0] = 1;

    buffer1[1] = .5;
    buffer2[1] = -.5;
    expected_buffer[1] = 0;

    buffer1[2] = -.5;
    buffer2[2] = -.5;
    expected_buffer[2] = -1;

    buffer1[3] = -20;
    buffer2[3] = 3.43;
    expected_buffer[3] = -16.57;

    add_signals(&buffer1, &buffer2, &result_buffer);

    if(compare_buffers(&expected_buffer, &result_buffer, num_samples))
        return true;

    return false;
}

/*************
 * RUN TESTS *
 *************/

/*
 * Run all tests.
 */
bool run_tests()
{
    string names[1];
    int results[1];
    int test_num = 0;

    names[test_num] = "test add signals 1";
    results[test_num] = test_add_signals_1();
    test_num ++;

    print_test_results(names, results, test_num);

    if(all_tests_passed(results, test_num))
        return true;

    return false;
}
