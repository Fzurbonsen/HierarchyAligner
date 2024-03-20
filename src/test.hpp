// This is the header file for test.cpp containing all functions and classes needed to run the tests.

#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;


// PRE:     fileName, testCases{TODO: struct/class that can hold the test cases}
//      fileName:       String containing the path to test case graph file.
//      testCases:      Instance of {testCase struct/class}.
// POST:    testCases{TODO: struct/class that can hold the test cases}
//      testCases:      Instance of {testCase struct/class} that holds the loaded test cases. 
void read_graph_file(string fileName);


// PRE:     fileName, testCases{TODO: struct/class that can hold the test cases}
//      fileName:       String containing the path to test case cluster file.
//      testCases:      Instance of {testCase struct/class}.
// POST:    testCases{TODO: struct/class that can hold the test cases}
//      testCases:      Instance of {testCase struct/class} that holds the loaded test cases.
void read_cluster_file(string fileName);


// PRE:     fileName, read_vector, graph_vector
//      fileName:       String containing the path to a file that holds the neccesary information
//                      to build the test graphs and reads.
//      read_vector:    Empty vector that has the string type.
//      graph_vector:   Empty vector that has the graph type.
// POST:    read_vector, graph_vector
//      read_vector:    Vector containing the reads for each test alignment.
//      graph_vector:   Vector containing the graphs for each test alignment.
//      return:         Integer value of the size of the test vectors.
int import_tests(string fileName, vector<string>& read_vector);


// PRE:     gold_vector, read_vector, graph_vector
//      gold_vector:    Empty vector that has the string type.
//      read_vector:    Vector holding the reads for each test alignment.
//      graph_vector:   Vector holding the graphs for each test alignment.
// POST:    gold_vector
//      gold_vector:    Vector holing the gold results for each test alignment.
void gold_results(vector<string>& gold_vector, vector<string>& read_vector);


// PRE:     test_vector, read_vector, graph_vector
//      test_vector:    Empty vector that has the string type.
//      read_vector:    Vector holding the reads for each test alignment.
//      graph_vector:   Vector holding the graphs for each test alignment.
// POST:    test_vector
//      test_vector:    Vector holing the test results for each test alignment.
void test_results(vector<string>& result_vector, vector<string>& read_vector);


// PRE:     n_tests, correct_tests, gold_vector, result_vector
//      n_tests:        Integer holding the numer of test cases in the test vector.
//      correct_tests:  Integer of value 0.
//      gold_vector:    Vector holing the gold results for each test alignment.
//      test_vector:    Vector holing the test results for each test alignment.
// POST:    correct_tests
//      correct_tests:  Integer holding the amount of test cases where the gold and test vector
//                      hold the same result.
void compare_results(int& n_tests, int& correct_tests, 
                        vector<string>& gold_vector, vector<string>& result_vector);

// PRE:     fileName
//      fileName:       String containing the path to a file that holds the neccesary information
//                      to build the test graphs and reads. 
// POST:
int run_tests(string fileName);