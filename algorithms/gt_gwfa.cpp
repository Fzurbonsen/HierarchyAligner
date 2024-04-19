/*

    This file holds the implementation of the connector between projectA and gt_gwfa.
    Author: Frederic zur Bonsen <fzurbonsen@student.ethz.ch>

*/
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <tuple>
#include <unordered_map>

#include "algorithms/gt_gwfa.hpp"
#include "gt_gwfa/graphs.h"
#include "algorithm.hpp"
#include "graph.hpp"

using namespace std;

// Constructor for parameter struct
projectA_gt_gwfa_parameters_t::projectA_gt_gwfa_parameters_t(const char* r) : read(r) {}


// Function to convert projectA_hash_graph_t into gssw_graph
gssw_graph* projectA_hash_graph_to_gt_gssw_graph(projectA_hash_graph_t* in_graph) {

    // Create gssw graph
    gssw_graph* out_graph = gssw_graph_create(in_graph->n_nodes);

    unordered_map<projectA_node_t*, gssw_node*> node_map;

    // Define parameters for gssw
    int8_t match = 1, mismatch = 4;
    uint8_t gap_open = 6, gap_extension = 1;
    gssw_sse2_disable();
    int8_t* nt_table = gssw_create_nt_table();
    int8_t* mat = gssw_create_score_matrix(match, mismatch);
    gssw_node* nodes[in_graph->n_nodes];

    // Iterate over all nodes in the graph to create the corresponding gssw nodes
    int i = 0;
    for (auto& curr_node : in_graph->nodes) {
        auto& curr = curr_node.second;

        // Fill node
        nodes[i] = (gssw_node*)gssw_node_create(NULL, curr->index, curr->seq.c_str(), nt_table, mat);

        // Add node to graph
        gssw_graph_add_node(out_graph, nodes[i]);

        // Add to node map
        node_map[curr] = nodes[i];
        i++;
    }

    // Check if graph sizes match
    if (out_graph->size != in_graph->n_nodes) {
        cerr << "Error: graph size does not match!\n";
        exit(1);
    }

    // Iterate over all edges
    for (auto& edge : in_graph->edges) {
        // Add edge
        gssw_nodes_add_edge(node_map[in_graph->nodes[edge.start]], node_map[in_graph->nodes[edge.end]]);
    }

    free(nt_table);
    free(mat);
    return out_graph;
}


// Function to initialize gt_gwfa
void* projectA_gt_gwfa_init(vector<pair<const string, projectA_hash_graph_t*>>& graphs) {
    // Create our input vector
    projectA_gt_gwfa_io_t* out = new projectA_gt_gwfa_io_t;

    // Iterate over input vector
    for (auto& itr : graphs) {
        projectA_gt_gwfa_parameters_t entry(itr.first.c_str());

        // Create gssw graph
        gssw_graph* new_gssw = projectA_hash_graph_to_gt_gssw_graph(itr.second);

        // Fill entry
        entry.gssw = new_gssw;
        entry.gwf = gt_gssw_to_gwf(new_gssw);

        // Append to vector
        out->parameters.push_back(entry);
    }

    // Reserve space for results
    out->gms.reserve(out->parameters.size());

    return static_cast<void*>(out);
}


// Function to calculate a batch in gt_gwfa
void* projectA_gt_gwfa_calculate_batch(void* input_ptr) {

    if (input_ptr == nullptr) {
        cerr << "Error: input is nullptr!\n";
        exit(1);
    }

    auto input = static_cast<projectA_gt_gwfa_io_t*>(input_ptr);

    // Iteratir over all parameters and run the algorithm
    int i = 0;
    for (auto& entry : input->parameters) {
        // gt_print_gssw_graph(entry.gssw);
        // cerr << i << endl;
        
        input->gms.push_back(gt_read_align_gwf(entry.gwf, entry.gssw, entry.read));
        // cerr << gt_read_align_gwf(entry.gwf, entry.gssw, entry.read) << endl;

        if (input->gms[i] == nullptr) {
            cerr << "Error: gms is nullptr at:\t" << i << endl;
            exit(1);
        }
        // gssw_print_graph_mapping(input->gms[i], stderr);
        // cerr << endl;

        gt_gwf_free(input->parameters[i].gwf);
        gssw_graph_destroy(input->parameters[i].gssw);
        gssw_graph_mapping_destroy(input->gms[i]);
        i++;
    }
    
    // Create output pointer
    return input_ptr;
}


// Function to handle post of gt_gwfa
void projectA_gt_gwfa_post(void* input_ptr) {

    if (input_ptr == nullptr) {
        cerr << "Error: input is nullptr!\n";
        exit(1);
    }

    auto input = static_cast<pair<vector<projectA_gt_gwfa_parameters_t>*, vector<gssw_graph_mapping*>*>*>(input_ptr);
    auto& parameters = *(input->first);
    auto& mappings = *(input->second);

    // TODO: Create usable results

    // Free memory allocated for mappings
    for (auto& mapping : mappings) {
        gssw_graph_mapping_destroy(mapping);
    }

    // Free memory allocated for parameters
    for (auto& parameter : parameters) {
        gt_gwf_free(parameter.gwf);
        gssw_graph_destroy(parameter.gssw);
    }

    // Free the input pointer itself
    delete input;
}

// Function to get the algorithm struct for gt_gwfa
void projectA_get_gt_gwfa(projectA_algorithm_t& gt_gwfa) {

    // Assign function pointers with correct signatures
    gt_gwfa.init = projectA_gt_gwfa_init;
    gt_gwfa.calculate_batch = projectA_gt_gwfa_calculate_batch;
    gt_gwfa.post = projectA_gt_gwfa_post;
}