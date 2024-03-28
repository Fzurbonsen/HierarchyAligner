#include <cstring>
#include <iostream>
#include <utility>
#include <set>

#include "graph.hpp"

using namespace std;


// Function to delete a node
void projectA_delete_node(projectA_node_t* node) {
    if (node != nullptr) {
        delete node;
    }
}


// Function to delete a graph
void projectA_delete_graph(projectA_graph_t* graph) {
    if (graph != nullptr) {
        delete graph;
    }
}


// Function to delete a hash graph
void projectA_delete_hash_graph(projectA_hash_graph_t* graph) {
    if (graph != nullptr) {
        for (auto& node : graph->nodes) {
            projectA_delete_node(node.second);
        }
        delete graph;
    }
}


// Function to append a node to a projectA_graph_t
void projectA_graph_append_node(projectA_graph_t* graph, uint32_t id, uint32_t len, string seq) {

    // Check if input seq matches the supposed length
    if (len != seq.size()) {
        cerr << "Error: sequence does not match sequence length!" << endl;
        return;
    }

    // add the node
    graph->id.push_back(id);
    graph->len.push_back(len);
    graph->seq.push_back(seq);

    // increase the node count
    graph->n_nodes++;
}


// Function to append an edge to a projectA_graph_t
void projectA_graph_append_edge(projectA_graph_t* graph, uint32_t start, uint32_t end) {

    // Initialize edge struct
    projectA_edge_t edge;
    edge.start = start;
    edge.end = end;

    // Append edge
    graph->edges.push_back(edge);

    // Increase the edge count
    graph->n_edges++;
}

// Function to append a node to a projectA_hash_graph_t
void projectA_hash_graph_append_node(projectA_hash_graph_t* graph, uint32_t id, uint32_t len, string& seq) {

    // Check if input seq matches the supposed length
    if (len != seq.size()) {
        cerr << "Error: sequence does not match sequence length!" << endl;
        return;
    }

    projectA_node_t* node = new projectA_node_t;

    // Fill node
    node->id = id;
    node->len = len;
    node->seq = seq;

    // Add node information to node map
    graph->nodes[id] = node;

    // Increase node counter
    graph->n_nodes++;
}


// Function to append an edge to a projectA_hash_graph_t
void projectA_hash_graph_append_edge(projectA_hash_graph_t* graph, uint32_t start, uint32_t end) {

    // Create a new edge
    projectA_edge_t edge;
    edge.start = start;
    edge.end = end;

    // Append edge to the graph
    graph->edges.push_back(edge);

    // Add edges to the nodes
    graph->nodes[start]->next.push_back(graph->nodes[end]);
    graph->nodes[end]->prev.push_back(graph->nodes[start]);

    // Increase edge counter
    graph->n_edges++;
}


// Function to store traversed nodes in the forward direction 
void projectA_graph_traverse_forward_distance(projectA_node_t* node , set<projectA_node_t*>& nodes, int64_t len) {

    // Check if we still need to traverse further
    if (len > 0) {

        len = len - node->len;
        nodes.insert(node);

        for (int i = 0; i < node->next.size(); ++i) {
            projectA_graph_traverse_forward_distance(node->next[i], nodes, len);
        }
    }
}


// Function to store traversed nodes in the backward direction 
void projectA_graph_traverse_backward_distance(projectA_node_t* node , set<projectA_node_t*>& nodes, int64_t len) {

    // Check if we still need to traverse further
    if (len > 0) {

        nodes.insert(node);
        len = len - node->len;

        for (int i = 0; i < node->prev.size(); ++i) {
            projectA_graph_traverse_forward_distance(node->prev[i], nodes, len);
        }
    }
}


// Function to build cluster information
void projectA_build_graph_from_cluster(projectA_hash_graph_t* graph, projectA_hash_graph_t* ref_graph, 
                                        projectA_node_list_t& node_list) {
    
    // cerr << node_list.n_nodes << endl;

    // Append nodes to graph
    for (int i = 0; i < node_list.n_nodes; ++i) {
        uint32_t& id = ref_graph->nodes[node_list.nodes[i]]->id;
        uint32_t& len = ref_graph->nodes[node_list.nodes[i]]->len;
        string& seq = ref_graph->nodes[node_list.nodes[i]]->seq;
        projectA_hash_graph_append_node(graph, id, len, seq);
    }

    // Iterate over all nodes to extract the contained edges.
    for (int i = 0; i < node_list.n_nodes; ++i) {
        const auto& node = ref_graph->nodes[node_list.nodes[i]];

        // Iterate over all edges linked to the node in the reference graph
        for (const auto& next : node->next) {
            
            // If the next node is included in the graph we add the edge.
            if (graph->nodes.find(next->id) != graph->nodes.end()) {
                projectA_hash_graph_append_edge(graph, node->id, next->id);
            }
        }
    }

    // cerr << graph->n_edges << "\t" << graph->n_nodes << endl;
}


// Function to build a vector of cluster information
void projectA_build_graph_from_cluster(vector<pair<string, projectA_hash_graph_t*>>& graphs, projectA_hash_graph_t* ref_graph, 
                                        vector<projectA_node_list_t>& node_lists) {
    
    // Iterate over all node lists
    for (auto& node_list : node_lists) {
        projectA_hash_graph_t* graph = new projectA_hash_graph_t;
        graph->n_edges = 0;
        graph->n_nodes = 0;

        // Build new graph for each node list
        projectA_build_graph_from_cluster(graph, ref_graph, node_list);
        
        // Append the graph to the graph vector
        graphs.push_back(make_pair(node_list.read, graph));
    }
}