/*

    This file holds the definition for the alorithm struct.
    Author: Frederic zur Bonsen <fzurbonsen@student.ethz.ch>

*/
#include <vector>
#include <utility>
#include <string>

#include "graph.hpp"

#ifndef PROJECTA_ALGORITHM_HPP
#define PROJECTA_ALGORITHM_HPP

// Strutct to hold the funciton calls to use an alignment algorithm.
struct projectA_algorithm_t {

    // Void pointer that holds the function pointer to initialize an algorithm. 
    // This function gets passed a vector of reads with corresponding graphs that should be aligned.
    // This function gets a void* return value that can be used to store any relevant information.
    void* (*init)(vector<pair<string, projectA_hash_graph_t*>>&); 

    // Void pointer that holds the function pointer to start the calculation of the initialized batch.
    // This function gets passed the void* return form the init function.
    // This funciotn gets a void* return to hold the results from the alignment.
    void* (*calculate_batch)(void*);

    // Void pointer that holds the function pointer to execute the post of the alignment.
    // This function gets passed the void* return from the calculate_batch function.
    // This funciont returns a vector that holds the results from the alignment.
    void (*post)(void*);

};


#endif // PROJECTA_ALGORITHM_HPP