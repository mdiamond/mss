/*
 * Matthew Diamond 2015
 * Functions for testing other functions.
 */

// Include header file
#include "tests.hpp"

using namespace std;

/********************
 * HELPER FUNCTIONS *
 ********************/

/*
 * Compare two buffers. Return 1 if they are the same,
 * 0 if they are not.
 */
int compare_buffers(vector<float> *buffer1, vector<float> *buffer2, int num_samples)
{
  int success = 1;

  for(int i = 0; i < num_samples; i ++)
  {
    if((*buffer1)[i] != (*buffer2)[i])
      success = 0;
  }

  return success;
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
int all_tests_passed(int *results, int test_num)
{
  int success = 1;

  for(int i = 0; i < test_num ; i ++)
  {
    if(!results[i])
      success = 0;
  }

  if(success)
    cout << "ALL TESTS PASSED!" << endl;
  else
    cout << "TESTS FAILED!" << endl;

  return success;
}

/*********
 * TESTS *
 *********/

/*
 * Test #1 for the add_signals() function.
 */
int test_add_signals_1(void)
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

  add_signals(&buffer1, &buffer2, &result_buffer, num_samples);

  if(compare_buffers(&expected_buffer, &result_buffer, num_samples))
    return 1;

  return 0;
}

/*************
 * RUN TESTS *
 *************/

/*
 * Run all tests.
 */
int run_tests(void)
{
  string names[1];
  int results[1];
  int test_num = 0;

  names[test_num] = "test add signals 1";
  results[test_num] = test_add_signals_1();
  test_num ++;

  print_test_results(names, results, test_num);

  if(all_tests_passed(results, test_num))
    return 1;

  return 0;
}
