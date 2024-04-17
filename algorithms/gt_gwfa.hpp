/*

    This file holds the definitions for the connector between projectA and gt_gwfa.
    Author: Frederic zur Bonsen <fzurbonsen@student.ethz.ch>
    
*/
#include <vector>
#include <string>
#include <utility>
#include <tuple>

#include "gt_gwfa/graphs.h"
#include "algorithm.hpp"
#include "graph.hpp"

using namespace std;

#ifndef PROJECTA_GT_GWFA_HPP
#define PROJECTA_GT_GWFA_HPP

// Struct to hold inputs for gt_gwfa
struct projectA_gt_gwfa_parameters_t {
    gssw_graph* gssw;
    gwf_graph_t* gwf;
    const char* read;

    projectA_gt_gwfa_parameters_t(const char* r);
};


// Struct to hold the inputs and outputs for gt_gwfa
struct projectA_gt_gwfa_io_t {
    vector<projectA_gt_gwfa_parameters_t> parameters;
    vector<gssw_graph_mapping*> gms;
};


void* projectA_gt_gwfa_init(vector<pair<string, projectA_hash_graph_t*>>& graphs);

void* projectA_gt_gwfa_calcualte_batch(void* input_ptr);

void projectA_gt_gwfa_post(void* input_ptr);


// PRE:     gt_gwfa
//      gt_gwfa:    Empty porjectA_algorithm struct.
// POST:    gt_gwfa
//      gt_gwfa:    projectA_algorithm struct that holds the function pointers to use
//                  gt_gwfa init/calculate_batch/post.
void projectA_get_gt_gwfa(projectA_algorithm_t& gt_gwfa);

#endif // PROJECTA_GT_GWFA_HPP