#include <iostream>

#include "CoupledClusterDiagram.h"
#include "Diagram.h"

using std::cout;

void CoupledClusterDiagram::CheckIsValid() {    

    // Iterate over vertices to check for virtual and H vertices
    int n_virtual = 0;
    int n_Hvertex = 0;

    for (int i=0; i<vertices.size(); ++i) {

        if (vertices[i]->IsVirtual()) {
            n_virtual++;
            pos_virtual_vertex = i;
        }
        if (vertices[i]->IsHVertex()) {
            n_Hvertex++;
            pos_Hvertex = i;
        }
    }

    has_virtual_vertex = (n_virtual > 0);
    has_Hvertex = (n_Hvertex ==1);

    // A valid Coupled Cluster diagram must have exactly one H vertex
    // and can have at most one virtual vertex
    is_valid = (has_Hvertex) && (n_virtual <= 1);
    if (!is_valid) return;

    // Amplitudes can not be connected to each other, but just to H or the virtual vertex.
    for (int i=0; i<vertices.size(); ++i) {
        if ( !vertices[i]->IsAmplitude() ) continue;
        for (int j=i+1; j<vertices.size(); ++j) {
            if ( !vertices[j]->IsAmplitude() ) continue;
            if ( adjacency_matrix(i,j) + adjacency_matrix(j,i) > 0 ) { 
                is_valid = false; 
                return; 
            }
        }
    }

    // H vertex must be connected to all other vertices (except the external one, if present)
    for (int j=0; j<vertices.size(); ++j) { 
        if ( !vertices[j]->IsAmplitude()) continue;
        if (adjacency_matrix(pos_Hvertex,j) + adjacency_matrix(j,pos_Hvertex)==0 ) {
            is_valid = false;
            return;
        }
    }

    // We look for equivalent vertices. We check the connectivity to the H vertex 
    // and impose an ordering to avoid redundant diagrams.
    for (int i=0; i<vertices.size(); ++i) {
        if ( !vertices[i]->IsAmplitude() ) continue;
        int conn_i_H = adjacency_matrix(i, pos_Hvertex) + adjacency_matrix(pos_Hvertex, i);
        if (conn_i_H == 0) { is_valid = false; return; }

        for (int j=i+1; j<vertices.size(); ++j) {
            if ( !vertices[j]->IsAmplitude() ) continue;
            if ( !vertices[i]->IsEquivalentTo(vertices[j]) ) continue; 
            
            int conn_j_H = adjacency_matrix(j, pos_Hvertex) + adjacency_matrix(pos_Hvertex, j);
            // First count the n. of connections with the H vertex. If equal, count n. of lines directed towards H.
            if ( conn_i_H < conn_j_H ) { is_valid = false; }
            else if ( conn_i_H == conn_j_H && adjacency_matrix(i, pos_Hvertex)<adjacency_matrix(j, pos_Hvertex) ) { is_valid = false; }
            
            if (!is_valid) return;
        }
    }
    
    return;
}

bool CoupledClusterDiagram::GetConnectivity(const IntMat &adj) {
    
    bool connected = Diagram::GetConnectivity(adj);
    if (!connected) return false;
    
    // Additional checks specific to Coupled Cluster diagrams can be added here
    
    if (!has_Hvertex) return false; // Must have H vertex
    return connected;
}


vector<int> CoupledClusterDiagram::FindSkeletonStructure(const IntMat& mat) const {
    // Implement skeleton structure extraction for Coupled Cluster diagrams
    // This is a placeholder implementation; actual logic will depend on the specific requirements for CC diagrams

    int n = mat.n_rows;
    vector<int> structure;

    // TODO HERE Implement

    return structure;
}