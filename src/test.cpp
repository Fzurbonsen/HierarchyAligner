#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#include "test.hpp"

using namespace std;

// Function to import tests form an external file
int import_tests(string fileName, vector<string>& read_vector) {
    // TODO
    return 0;
}

// Function to get gold results
void gold_results(vector<string>& gold_vector, vector<string>& read_vector)  {
    // TODO
}

// Function to get test results
void test_results(vector<string>& result_vector, vector<string>& read_vector) {
    // TODO
}

// Function to compare the test and gold results
void compare_results(int& n_tests, int& correct_tests, 
                        vector<string>& gold_vector, vector<string>& result_vector) {
    
    // Ensure that the gold vector has the size of the number of tests
    if (gold_vector.size() != n_tests) {
        cerr << "Error: Gold size does not match n_tests\n";
    }

    // Ensure that the result vector has the size of the number of tests
    if (result_vector.size() != n_tests) {
        cerr << "Error: Result size does not match n_tests\n";
    }
    
    // Iterate through the vectors and compare the elements
    for (size_t i = 0; i < n_tests; ++i) {
        if (gold_vector[i] == result_vector[i]) {
            // If the elements match we increment the counter
            correct_tests++;
        } else {
            // Print the mismatched test case
            cerr << "Mismatch at index " << i << ":\n";
            cerr << "Gold result: " << gold_vector[i] << "\n";
            cerr << "Result:      " << result_vector[i] << "\n";
        }
    }
}


// Function that handles all the test cases
int run_tests(string fileName) {
    cerr << "running tests from file: " << fileName << "\n";
    
    // Variables used to track performance
    double runtime;
    int n_tests, correct_tests = 0;

    // record the starting time
    clock_t start = clock();


    // Vector filled with graphs
    // vector<"graph_class"> graph_vector;
    // Vector filled with the reads
    vector<string> read_vector;
    // // Vector for gold and normal results
    vector<string> result_vector, gold_vector;

    // Load file with test graphs and alignments
    n_tests = import_tests(fileName, read_vector);

    // Get gold results
    gold_results(gold_vector, read_vector);

    // Get test results
    test_results(result_vector, read_vector);

    // Compare the tests and gold standard.
    compare_results(n_tests, correct_tests, gold_vector, result_vector);
    
    // record the ending time
    clock_t end = clock();
    // calculate the runtime in seconds
    runtime = double(end - start) / CLOCKS_PER_SEC;

    // output the results of the tests
    cerr << "time: " << runtime << " seconds\n" << correct_tests << " out of " << n_tests << " are correct\n";

    return 0;
}