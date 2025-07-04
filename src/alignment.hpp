/*

    projectA:
    alignment.hpp
    This file holds the definitions for the projectA alignment struct and its sub structs
    as well as the definitions of helperfunctions to work with the structs.
    Author: Frederic zur Bonsen <fzurbonsen@student.ethz.ch>

*/

#include <vector>
#include <utility>
#include <unordered_map>
#include <set>
#include <tuple>
#include <string>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <sstream>
#include <unordered_set>
#include <cstdint>

#include "graph.hpp"

using namespace std;

#ifndef PROJECTA_ALIGNMENT_HPP
#define PROJECTA_ALIGNMENT_HPP



// Struct that holds a CIGAR element and how often it is repeated
struct projectA_cigar_element_t {
    uint32_t len; // Number of consecutive repetitions of the element
    char type; // Type of the element
};


// Struct that holds a complete CIGAR, being an array of cigar elements
struct projectA_cigar_t {
    uint32_t len; // Number of CIGAR elements in the array
    int32_t operations_length; // Number of operations in the CIGAR
    vector<projectA_cigar_element_t> elements; // Vector of CIGAR elements
};

 
// Struct to hold an alignment
struct projectA_alignment_t {
    int32_t id; // Identifier of the alignment
    // Inputs
    string read; // Read to be aligned
    projectA_hash_graph_t* graph; // Graph on which to align the read
    int8_t match;
    int8_t mismatch;
    uint8_t gap_open;
    uint8_t gap_extend;
    // Intermediate
    string reference;
    // Outputs
    uint32_t offset; // Offset in the first node
    int32_t score;  // Alignment score
    uint32_t size; // Number of nodes included in the alignment
    vector<string> nodes; // In order vector of node ids that are included in the alignment
    vector<projectA_cigar_t> cigar; // Vector of CIGAR elements with position corresponing to nodes in the nodes vector
    projectA_cigar_t cigar_string; // CIGAR over all nodes

    uint32_t read_start; // Offset in read for local alignment
    int32_t n_matches; // Number of matches in the CIGAR
    int32_t n_mismatches; // Number of mismatches in the CIGAR
};


// PRE:     cigar
//      cigar:          String that holds a valid CIGAR
// POST:    return
//      return:         projectA_cigar_t struct that holds the CIGAR.
projectA_cigar_t projectA_parse_cigar_string(const string& cigar_str);


// PRE:     cigar1, cigar2
//      cigar1:         Pointer to a projectA CIGAR struct.
//      cigar2:         Pointer to a projectA CIGAR struct.
// POST:    return
//      cigar1:         Pointer to concatenated CIGAR struct. 
void projectA_concat_cigar(projectA_cigar_t* cigar1, projectA_cigar_t* cigar2);


// PRE:     cigar1, cigar2
//      cigar1:         Pointer to a projectA CIGAR struct.
//      cigar2:         Pointer to a projectA CIGAR struct.
// POST:    return
//      return:         Double of the relative accuracy of the two CIGARs. 
double projectA_cigar_accuracy(projectA_cigar_t* cigar1, projectA_cigar_t* cigar2);


// PRE:     nodes1, nodes2
//      nodes1:         Reference to a vector of nodes.
//      nodes2:         Reference to a vector of nodes.
// POST:    return
//      return:         Double of the relative accuracy of the two paths. 
double projectA_path_accuracy(vector<string>& nodes1, vector<string>& nodes2);


// PRE:     nodes1, nodes2
//      nodes1:         Pointer to a vector of nodes and their length.
//      nodes2:         Pointer to a vector of nodes and their length.
// POST:    return
//      return:         Double of the relative weighted accuracy of the two paths. 
double project_weighted_path_accuracy(projectA_alignment_t* alignment1, projectA_alignment_t* alignment2);


// PRE:     path, cutoff
//      path:           Reference to a vector of strings that hold a node path.
//      cutoff:         Reference to a string that indicates the cutoff node.
// POST:    path
//      path:           Reference to a vector of strings that hold a node path
//                      that has been cut at cutoff if cutoff is reached.
void projectA_cut_path(vector<string>& path, const string& cutoff);


// PRE:     cigar1, cigar2
//      cigar1:         Reference to a projectA CIGAR struct.
//      cigar2:         Reference to a projectA CIGAR struct.
// POST:    return
//      return:         Boolean value indicating whether the two CIGARSs are identical. 
bool projectA_compare_cigar(projectA_cigar_t& cigar);


// PRE:     print, file, alignment1, alignment2
//      print:          Boolean that indicates whether to print the results to file or not.
//      file:           Pointer to a valid output file if print is enabled.
//      alignment1:     Pointer to a valid projectA alignment struct.
//      alignment2:     Pointer to a valid projectA alignment struct.
// POST:    return, file
//      return:         Integer that is one if the alignments match and zero if they don't.
//      file:           Pointer to a valid output file that holds that holds the information to what
//                      degree the two alignments match.
int  projectA_compare_alignments(bool print, FILE* file, projectA_alignment_t* alignment1, 
                                                projectA_alignment_t* alignment2);


// PRE:     print, file, alignment1, alignment2
//      print:          Boolean that indicates whether to print the results to file or not.
//      file:           Pointer to a valid output file if print is enabled.
//      alignment1:     Pointer to a valid projectA alignment struct.
//      alignment2:     Pointer to a valid projectA alignment struct.
// POST:    return, file
//      return:         Integer that is one if the alignments match and zero if they don't.
//      file:           Pointer to a valid output file that holds that holds the information to what
//                      degree the two alignments match.
int projectA_compare_alignments_path(bool print, FILE* file, projectA_alignment_t* alignment1, 
                                                            projectA_alignment_t* alignment2);


// PRE:     nodes_sim, nodes_aln
//      nodes_sim:      Vector that holds the simulated position.
//      nodes_aln:      Vector that holds the aligned position.
// POST:    return
//      return:         Float that holds the relative content of the simulated position in the aligned position.
float projectA_node_sub_set(const vector<string>& nodes_sim, const vector<string>& nodes_aln);


#endif  //PROJECTA_ALIGNMENT_HPP