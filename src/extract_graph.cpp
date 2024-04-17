/*

    This file holds the implementation of an algorithm to construct a graph from a position
    this algorithm is a heavily simplified version of a similar algorithm in vg this implementation can
    be used to create a graph for projectA.
    Author: Frederic zur Bonsen <fzurbonsen@student.ethz.ch>
    
*/

#include <string>
#include <vector>
#include <iostream>
#include <utility>
#include <algorithm>
#include <set>
#include <cstdint>

#include "extract_graph.hpp"

using namespace std;

// Function to extract a graph from the positions
vector<pair<string, set<projectA_node_t*>>> projectA_extract_graph(projectA_hash_graph_t* ref_graph, vector<projectA_position_t>& positions) {

    // Initialize vector that will be returned and auxiliary pair
    vector<pair<string, set<projectA_node_t*>>> node_lists;

    // Reserve space for the vector
    node_lists.reserve(positions.size());

    // Iterate over the positions vector to get the nodes for each position
    for (auto& position : positions) {
        node_lists.emplace_back(position.read, projectA_extract_nodes(ref_graph, position));
    }

    return node_lists;
}


// Functino to extract nodes from a position
set<projectA_node_t*> projectA_extract_nodes(projectA_hash_graph_t* ref_graph, projectA_position_t& pos) {
    set<projectA_node_t*> nodes;

    if (pos.forward_search_lengths.size() != pos.backward_search_lengths.size() 
        || pos.forward_search_lengths.size() != pos.id.size()
        || pos.forward_search_lengths.size() != pos.is_reverse.size()
        || pos.forward_search_lengths.size() != pos.offset.size()) {
        cerr << "Error: extraction search length does not math positions" << endl;
        exit(1);
    }

    // Calculate the max search length
    int64_t max_search_length = max(*max_element(pos.forward_search_lengths.begin(), pos.forward_search_lengths.end()),
                                    *max_element(pos.backward_search_lengths.begin(), pos.backward_search_lengths.end()));

    // Iterate over all positions and calculate the reachable nodes
    for (size_t i = 0; i < pos.id.size(); ++i) {

        // Initialize local variables for readability
        const string& id = pos.id[i];
        const bool& is_reverse = pos.is_reverse[i];
        const uint32_t& offset = pos.offset[i];
        const uint32_t& forward_search_length = pos.forward_search_lengths[i];
        const uint32_t& backward_search_length = pos.backward_search_lengths[i];

        int32_t dist_forward = offset + forward_search_length + max_search_length;
        int32_t dist_backward = offset + backward_search_length + max_search_length;

        // cerr << offset << "\t" << forward_search_length << "\t" << backward_search_length << "\t" << max_search_length << endl;
        // cerr << dist_forward << "\t" << dist_backward << endl;

        projectA_graph_traverse_forward_distance(ref_graph->nodes[id], nodes, dist_forward);
        projectA_graph_traverse_backward_distance(ref_graph->nodes[id], nodes, dist_backward);

    }


    // for (auto it = nodes.begin(); it != nodes.end(); ++it) {
    //     cerr << (*it)->id << " ";
    // }
    // cerr << endl;
    return nodes;
}