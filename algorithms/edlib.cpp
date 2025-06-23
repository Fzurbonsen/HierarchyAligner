/*

    projectA:
    edlib.cpp
    This file holds the implementation of the connector between projectA and edlib.
    Author: Frederic zur Bonsen <fzurbonsen@student.ethz.ch>

*/
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "algorithms/edlib.hpp"
#include "file_io.hpp"

using namespace std;

void projectA_edlib(projectA_alignment_t* alignment) {

    // Define local variables
    auto& read = alignment->read;
    auto& reference = alignment->reference;
    auto& cigar = alignment->cigar_string;
    cigar.elements.clear();
    cigar.len = 0;

    EdlibAlignResult result = edlibAlign(read.c_str(), strlen(read.c_str()), 
                                            reference.c_str(), strlen(reference.c_str()), 
                                            edlibNewAlignConfig(-1, EDLIB_MODE_HW, EDLIB_TASK_PATH, NULL, 0));
    char* edlib_cigar = edlibAlignmentToCigar(result.alignment, result.alignmentLength, EDLIB_CIGAR_STANDARD);
    string cigar_str = edlib_cigar;
    int32_t score = result.editDistance;
    free(edlib_cigar);
    edlibFreeAlignResult(result);
    cigar = projectA_parse_cigar_string(cigar_str);
    alignment->offset = 0;
    alignment->score = -score;
}

void projectA_edlib_threaded(projectA_alignment_t* alignment) {

    // Define local variables
    auto& read = alignment->read;
    auto& reference = alignment->reference;
    auto& cigar = alignment->cigar_string;
    cigar.elements.clear();
    cigar.len = 0;

    EdlibAlignResult result = edlibAlign(read.c_str(), strlen(read.c_str()), 
                                            reference.c_str(), strlen(reference.c_str()), 
                                            edlibNewAlignConfig(-1, EDLIB_MODE_HW, EDLIB_TASK_PATH, NULL, 0));
    char* edlib_cigar = edlibAlignmentToCigar(result.alignment, result.alignmentLength, EDLIB_CIGAR_STANDARD);
    string cigar_str = edlib_cigar;
    int32_t score = result.editDistance;
    free(edlib_cigar);
    edlibFreeAlignResult(result);
    cigar = projectA_parse_cigar_string(cigar_str);
    alignment->offset = 0;
    alignment->score = -score;
}

// Constructor for parameter struc
projectA_edlib_parameters_t::projectA_edlib_parameters_t(string read, string reference, projectA_cigar_t cigar, int32_t score) :
                                                        read(read), reference(reference), cigar(cigar), score(score) {};


// Function to initialize edlib
void* projectA_edlib_init(vector<projectA_alignment_t*>& alignments, int32_t numThreads) {

    // Create io vectors for the edlib algorithm
    projectA_edlib_io_t* out = new projectA_edlib_io_t;
    for (int i = 0; i < numThreads; ++i) {
        vector<projectA_edlib_parameters_t> params;
        params.reserve(alignments.size()/numThreads);
        out->parameters.push_back(params);
    }

    out->size = alignments.size();

    int32_t thread_index = 0;
    for (auto& itr : alignments) {
        projectA_edlib_parameters_t entry(itr->read, itr->reference, itr->cigar_string, 0);
        entry.cigar.elements.clear();
        entry.cigar.len = 0;

        out->parameters[thread_index].push_back(entry);

        thread_index = (thread_index == numThreads - 1) ? 0 : thread_index + 1;
    }

    return static_cast<void*>(out);
}


// Function to claculate a batch in edlib
void* projectA_edlib_calculate_batch(void* ptr, int32_t thread_index) {

    // Check if ptr is valid
    if (ptr == nullptr) {
        cerr << "Error: input is nullptr!\n";
        exit(1);
    }

    // Define and cast local variables
    auto input = static_cast<projectA_edlib_io_t*>(ptr);
    auto& parameters = input->parameters[thread_index];

    // Run edlib on every set of parameters
    for (int i = 0; i < parameters.size(); ++i) {
        auto& parameter = parameters[i];
        parameter.result = edlibAlign(parameter.read.c_str(), strlen(parameter.read.c_str()), 
                                            parameter.reference.c_str(), strlen(parameter.reference.c_str()), 
                                            edlibNewAlignConfig(-1, EDLIB_MODE_HW, EDLIB_TASK_PATH, NULL, 0));
    }

    return static_cast<void*>(ptr);
}


// Function to handle post of edlib
void projectA_edlib_post(void* ptr, vector<projectA_alignment_t*>& alignments, int32_t numThreads) {

    // Check if ptr is valid
    if (ptr == nullptr) {
        cerr << "Error: input is nullptr!\n";
        exit(1);
    }

    // Define local variables
    auto input = static_cast<projectA_edlib_io_t*>(ptr);
    auto& parameters_vec = input->parameters;

    // Iterate over all parameters to reconstruct the results
    int32_t thread_index = 0;
    int32_t j = 0;
    for (int32_t i = 0; i < input->size; ++i) {
        auto& parameters = parameters_vec[thread_index];
        
        // Check for out of bounds error 
        if (j >= parameters.size()) {
            cerr << "Error: The index is out of bound!\n";
            cerr << "index: " << j << "\tin vector of the thread: " << thread_index << endl;
            cerr << "at value: " << i << endl;
            exit(1);
        }

        // Update alignment
        char* edlib_cigar = edlibAlignmentToCigar(parameters[j].result.alignment, parameters[j].result.alignmentLength, EDLIB_CIGAR_STANDARD);
        string cigar_str = edlib_cigar;
        int32_t score = parameters[j].result.editDistance;
        free(edlib_cigar);
        edlibFreeAlignResult(parameters[j].result);
        alignments[i]->cigar_string.elements.clear();
        alignments[i]->cigar_string.len = 0;
        alignments[i]->cigar_string = projectA_parse_cigar_string(cigar_str);
        alignments[i]->offset = 0;
        alignments[i]->score = -score;

        // Update thread index
        if (thread_index == numThreads - 1) {
            thread_index = 0;
            ++j;
        } else {
            ++thread_index;
        }
    }

    delete input;
    ptr = nullptr;
}


projectA_algorithm_t* projectA_get_edlib() {
    projectA_algorithm_t* edlib = new projectA_algorithm_t;

    edlib->init = projectA_edlib_init;
    edlib->calculate_batch = projectA_edlib_calculate_batch;
    edlib->post = projectA_edlib_post;

    return edlib;
}


// Function to delete projectA_algirhtm_t struct
void projectA_edlib_destroy(projectA_algorithm_t* edlib) {

    if (edlib == nullptr) {
        cerr << "Warning: edlib_gwfa struct is null pointer!\n";
        return;
    }

    // Delete struct
    delete edlib;
}


