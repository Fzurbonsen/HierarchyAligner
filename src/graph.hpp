/*

    This file holds the definitions for the projectA graph structures and it's sub structs
    as well as helper functions to work with the structs.
    Author: Frederic zur Bonsen <fzurbonsen@student.ethz.ch>

*/

#include <vector>
#include <utility>
#include <unordered_map>
#include <set>

using namespace std;


#ifndef PROJECTA_GRAPH_HPP
#define PROJECTA_GRAPH_HPP

// Struct that holds a CIGAR element (A,C,G,T) and how often it is repeated
struct projectA_cigar_element_t {
    uint32_t len; // Number of consecutive repetitions of the element
    char type; // Type of the element
};


// Struct that holds a complete CIGAR, being an array of cigar elements
struct projectA_cigar_t {
    uint32_t len; // Number of CIGAR elements in the array
    projectA_cigar_element_t* elements; // Array of CIGAR elements
};


struct projectA_edge_t {
    string start; // Start index of the edge
    string end; // End index of the edge
};


// Struct to hold a node
struct projectA_node_t {
    string id; // Node id
    uint32_t index; // Node index
    uint32_t len; // Node sequence length
    string seq; // Sequence stored in node
    projectA_cigar_t cigar; // CIGAR corresponding to node
    vector<projectA_node_t*> next; // Next nodes
    vector<projectA_node_t*> prev; // Previous nodes

    bool visited = false; // Auxiliary bool used to speed up certain functions 
};


// Struct to hold reference graph
struct projectA_graph_t {
    uint32_t n_nodes; // number of nodes
    uint32_t n_edges; // number of edges

    // Nodes
    vector<string> id; // Vector of node ids
    vector<uint32_t> len; // Vector of sequence lengths
    vector<string> seq; // Vector of sequences 
    vector<projectA_cigar_t> cigars; // Vector of CIGARs

    // Edges
    vector<projectA_edge_t> edges; // Vector of edges in the graph 
};


// Struct to hold hashed graph
struct projectA_hash_graph_t {
    uint32_t n_nodes; // Number of nodes
    uint32_t n_edges; // Number of edges
    unordered_map<string, projectA_node_t*> nodes; // Map storing nodes for easy access
    vector<projectA_node_t*> nodes_in_order;

    projectA_node_t* top_node = nullptr; // Node that has only outgoing edges and from which we can reach any node

    vector<projectA_edge_t> edges;
};


// Struct to hold positions form vg
struct projectA_position_t {
    vector<string> id; // Vecotr holding the different position ids
    vector<bool> is_reverse; // Vector holding the orientation of the positions
    vector<uint32_t> offset; // Vector holding the offset of the different positions
    vector<size_t> forward_search_lengths;
    vector<size_t> backward_search_lengths;

    string read; // Read corresponding to the positions
};


// Struct to hold a node list
// A node list in this context is a vector of node ids paired with a read 
// where the read should be aligned to the graph created by the nodes
// in the reference graph.
struct projectA_node_list_t {
    uint32_t n_nodes; // Number of nodes
    vector<string> nodes; // Vector of node ids

    size_t read_len; // Length of the read
    string read; // Read corresponding to the nodes
};


void projectA_delete_node(projectA_node_t* node);
void projectA_delete_graph(projectA_graph_t* graph);
void projectA_delete_hash_graph(projectA_hash_graph_t* graph);

// PRE:     graph, id, len, seq
//      graph:      Pointer to a projectA_graph_t.
//      id:         String that holds a unique id for the node.
//      len:        Unsigned int of size 32 bit that holds the length of the sequence to be appended.
//      seq:        String that holds the sequence to be stored in the node.
// POST:    graph
//      graph:      Pointer to a projectA_graph_t where the node with the specified id, len, seq has been added.
void projectA_graph_append_node(projectA_graph_t* graph, string id, uint32_t len, string seq);


// PRE:     graph, start, end
//      graph:      Pointer to a projectA_graph_t.
//      start:      Unsigned int of size 32 bit that holds the index of the start of the edge.
//      end:        Unsigned int of size 32 bit that holds the indes of the end of the edge.
// POST:    graph
//      graph:      Pointer to a projectA_graph_t where the edge with the specified start and end has been added.
void projectA_graph_append_edge(projectA_graph_t* graph, string start, string end);


// PRE:     graph, id, len, seq, index
//      graph:      Pointer to a projectA_hash_graph_t.
//      id:         String that holds a unique id for the node.
//      len:        Unsigned int of size 32 bit that holds the length of the sequence to be appended.
//      seq:        String that holds the sequence to be stored in the node.
//      index:      Uinsigned int of size 32 bit that holds a unique index for the node.
// POST:    graph
//      graph:      Pointer to a projectA_hash_graph_t where the node with the specified id, len, seq has been added.
void projectA_hash_graph_append_node(projectA_hash_graph_t* graph, string id, uint32_t len, string& seq, uint32_t index);


// PRE:     graph, start, end
//      graph:      Pointer to a projectA_hash_graph_t.
//      start:      Unsigned int of size 32 bit that holds the index of the start of the edge.
//      end:        Unsigned int of size 32 bit that holds the indes of the end of the edge.
// POST:    graph
//      graph:      Pointer to a projectA_hash_graph_t where the edge with the specified start and end has been added.
void projectA_hash_graph_append_edge(projectA_hash_graph_t* graph, string start, string end);


// PRE:     node, nodes, len
//      node:       Pointer to a projectA_node_t.
//      nodes:      Reference to a set of pointers of porjectA_node_t.
//      len:        Length of remaining referecne sequence.
// POST:    nodes
//      nodes:      Reference to a set of pointer of projectA_node_t that were traversed in forward direction.
void projectA_graph_traverse_forward_distance(projectA_node_t* node , set<projectA_node_t*>& nodes, int64_t len);


// PRE:     node, nodes, len
//      node:       Pointer to a projectA_node_t.
//      nodes:      Reference to a set of pointers of porjectA_node_t.
//      len:        Length of remaining referecne sequence.
// POST:    nodes
//      nodes:      Reference to a set of pointer of projectA_node_t that were traversed in backward direction.
void projectA_graph_traverse_backward_distance(projectA_node_t* node , set<projectA_node_t*>& nodes, int64_t len);


// PRE:     graph, ref_graph, node_list
//      graph:      Pointer to a projectA_hash_graph_t.
//      ref_graph:  Pointer to a projectA_hash_graph_t that holds the valid reference graph.
//      node_list:  ProjectA_node_list_t that holds the node ids of a cluster in the reference graph
//                  as well as the corresponding reads.
// POST:    graph
//      graph:      Pointer to a projectA_hash_graph_t that holds a graph corresponding to the node list.
void projectA_build_graph_from_cluster(projectA_hash_graph_t* graph, projectA_hash_graph_t* ref_graph, 
                                        projectA_node_list_t& node_list);


// PRE:     graphs, ref_graph, node_lists
//      graphs:     Vector holding a pair of a string and a pointer to a projectA_hash_graph_t.
//      ref_grapph: Pointer to a projectA_hash_graph_t that holds the valid reference graph.
//      node_list:  Vector that holds the projectA_node_list_t holding the cluster information
// POST:    graphs
//      graphs:     Vector holding a pair of a string and a pointer to a projectA_hash_graph_t. The string 
//                  holds the read while the projectA_hash_graph_t pointer points to the corresponding graph.
void projectA_build_graph_from_cluster(vector<pair<string, projectA_hash_graph_t*>>& graphs, projectA_hash_graph_t* ref_graph, 
                                        vector<projectA_node_list_t>& node_lists);


// PRE:     graph
//      graph:      Pointer to a projectA_hash_graph_t.
// POST:    grpah
//      grpah:      Pointer to a projectA_hash_graph_t that has been indexed.
void projectA_index_hash_graph(projectA_hash_graph_t* graph);


// PRE:     graph
//      graph:      Pointer to a valid projectA_hash_graph_t.
// POST:    graph
//      graph:      Pointer to a valid projectA_hash_graph_t where the top node has been updated.
void projectA_hash_graph_find_top_node(projectA_hash_graph_t* graph);


// PRE:     graph
//      graph:      Pointer to a valid projectA_hash_graph_t.
// POST:    graph
//      graph:      Pointer to a valid projectA_hash_graph_t where the nodes_in_order field has been updated
//                  with an in order vector of the nodes in the graph.
void projectA_hash_graph_in_order_nodes(projectA_hash_graph_t* graph);

#endif // PROJECTA_GRAPH_HPP