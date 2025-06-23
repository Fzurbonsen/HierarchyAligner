/*

    projectA:
    edlib.hpp
    This file holds the definitions for the connector between proejctA and edlib.
    Author: Frederic zur Bonsen <fzurbonsen@student.ethz.ch>

*/
#include <vector>
#include <string>

#include "edlib/edlib.h"
#include "algorithm.hpp"
#include "graph.hpp"
#include "alignment.hpp"

using namespace std;

#ifndef PROJECTA_EDLIB_HPP
#define PROJECTA_EDLIB_HPP

// PRE:     alignment
//      alignment:      Pointer to a projectA_alignment_t.
// POST:    alignment
//      alignment:      Pointer to a projectA_alignment with the full CIGAR of the given path.
void projectA_edlib(projectA_alignment_t* alignment);


// Struct to hold inputs for edlib 
struct projectA_edlib_parameters_t {
    string read;
    string reference;
    projectA_cigar_t cigar;
    EdlibAlignResult result;
    char* edlib_cigar;
    int32_t score;

    // Constructor for projectA_edlib_parameters_t
    projectA_edlib_parameters_t(string read, string reference, projectA_cigar_t cigar, int32_t score);
};

// IO struct for edlib
struct projectA_edlib_io_t {
    vector<vector<projectA_edlib_parameters_t>> parameters;
    int32_t size;
};

void* projectA_edlib_init(vector<projectA_alignment_t*>& alignments, int32_t numThreads);

void* projectA_edlib_calculate_batch(void* ptr, int32_t thread_index);

void projectA_edlib_post(void* ptr, vector<projectA_alignment_t*>& alignments, int32_t numThreads);

projectA_algorithm_t* projectA_get_edlib();

void projectA_edlib_destroy(projectA_algorithm_t* edlib);

#endif