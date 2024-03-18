#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>

#include "test.hpp"

using namespace std;

// Function to read contents of a graph file
void read_graph_file(string fileName) {

    // Initialise variables
    bool skip;
    int8_t state = 0; // 0 = default, 1 = graph, 2 = read
    ifstream file(fileName);
    string line, ignore, read;
    char type, dir;
    uint32_t id, id1, id2, seq_len;
    string sequence;

    // Check if file could be opened
    if (!file.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        exit(0);
    }

    // Read file
    while (getline(file, line)) {

        // Skip empty lines
        if (line.empty() || line[0] == '#') continue;

        // Check if we have a graph next
        if (line[0] == 'N' && line[1] == 'G' && state == 0) {
            state++;
        }
            
        // Check if line is node
        if (line[0] == 'S' && state == 1) {
            // Initialise string stream
            istringstream iss(line);
            
            // Read values from stream
            iss >> type >> id >> sequence;
            // Extract sequence length
            size_t length_index = line.find("LN:i:");
            if (length_index != string::npos) {
                istringstream length_stream(line.substr(length_index + 5));
                length_stream >> seq_len;
            }

            // Compare sequence length if they don't then we skip to the next graph
            if (sequence.length() != seq_len) {
                cerr << "Error: sequnce length does not match, skipping to next graph!\n";
                skip = true;
                break;
            }
            // TODO:
            // Create corresponding node
            cerr << type << "\t" << id << "\t" << sequence << "\t" << seq_len << endl;
        }

        // Check if line is edge
        if (line[0] == 'L' && state == 1) {
            // Initialise string stream
            istringstream iss(line);

            // Read values from stream
            iss >> type >> id1 >> dir >> id2 >> ignore;
            // TODO:
            // Create corresponding edges
            cerr << type << "\t" << id1 << "\t" << dir << "\t" << id2 << endl;
        }


        // Check if we have a read next
        if (line[0] == 'N' && line[1] == 'R' && state == 1) {
            state++;
        }

        // Check if line is read
        if (line[0] == 'R' && state == 2) {
            istringstream iss(line);
            iss >> type >> read;
            state = 0;
            // TODO:
            // Create corresponding reads
            cerr << type << "\t" << read << endl;
        }

        // Check for errors
        if ((state > 2) || (line[0] != 'N' && line[0] != 'S' && line[0] != 'L' && line[0] != 'R')) {
            cerr << "Error: Graph was not formated propperly, skipping to next graph!\n";
            state = 0;
        }
    }
}


void read_cluster_file(string fileName) {
    
    // Initialise variables
    bool skip;
    int8_t state = 0; // 0 = default, 1 = graph, 2 = read
    ifstream file(fileName);
    string line, read;
    char type;
    uint32_t id, is_reverse, offset, forward_max_dist, backward_max_dist;
    string sequence;

    // Check if file could be opened
    if (!file.is_open()) {
        cerr << "Error opening file: " << fileName << endl;
        exit(0);
    }

    // Read file
    while (getline(file, line)) {

        // Skip empty lines
        if (line.empty() || line[0] == '#') continue;

        // Check if we have a position next
        if (line[0] == 'N' && line[1] == 'C' && state == 0) {
            state++;
        }
            
        // Check if line is position
        if (line[0] == 'P' && state == 1) {
            // Initialise string stream
            istringstream iss(line);
            
            // Read values from stream
            iss >> type >> id >> is_reverse >> offset >> forward_max_dist >> backward_max_dist;

            // TODO:
            // Create corresponding positions
            cerr << type << "\t" << id << "\t" << is_reverse << "\t" << offset << "\t" << forward_max_dist << "\t" << backward_max_dist << endl;
        }


        // Check if we have a read next
        if (line[0] == 'N' && line[1] == 'R' && state == 1) {
            state++;
        }

        // Check if line is read
        if (line[0] == 'R' && state == 2) {
            istringstream iss(line);
            iss >> type >> read;
            state = 0;
            // TODO:
            // Create corresponding reads
            cerr << type << "\t" << read << endl;
        }

        // Check for errors
        if ((state > 2) || (line[0] != 'N' && line[0] != 'P' && line[0] != 'R')) {
            cerr << "Error: Position was not formated propperly, skipping to next Position!\n";
            state = 0;
        }
    }
}


// Function to import tests form an external file
int import_tests(string fileName, vector<string>& read_vector) {
    // read_graph_file(fileName);
    read_cluster_file(fileName);
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
        return;
    }

    // Ensure that the result vector has the size of the number of tests
    if (result_vector.size() != n_tests) {
        cerr << "Error: Result size does not match n_tests\n";
        return;
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