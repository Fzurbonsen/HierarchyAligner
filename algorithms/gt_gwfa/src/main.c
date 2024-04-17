#include <stdio.h>
#include "graphs.h"


int main(int argc, char *argv[]) {
    char * name = "test";
    char * const a = "TTTAGG";
    char * const b = "AGAGTAAGG";
    char * const c = "TTTTT";
    char * const d = "TAGAAG";
    char * const e = "TTTAGGAGAGTAAGGTAGAAG";
    char * const args1[] = {name, a, b, c, d, e};
    int8_t debug;

    gssw_graph *graph_1, *graph_2;
    gwf_graph_t *graph_3, *graph_4;

    const char* filename = "test.gfa";
    graph_2 = gt_read_gfa(filename);

    gt_print_gssw_graph(graph_2);

    // graph_2 = gt_import_gssw_graph(6, args1);
    graph_4 = gt_gssw_to_gwf_direct(graph_2);

    gssw_graph_mapping* gm = gt_read_align_gwf(graph_4, graph_2, e);
    gssw_graph_cigar* gc = &gm->cigar;
    gssw_node_cigar* ncs = gc->elements;

    gssw_print_graph_mapping(gm, stderr);

    FILE *file_fp = fopen("test_graphs.txt", "w");
    gt_gwfa_print_to_file(file_fp, graph_4);
    fclose(file_fp);

    gt_check_node_length(gm);
    gt_check_alignment_length(gm, strlen(e));
    gssw_graph_mapping_destroy(gm);
    gssw_graph_destroy(graph_2);
    gt_gwf_free(graph_4);

    return 0;
}