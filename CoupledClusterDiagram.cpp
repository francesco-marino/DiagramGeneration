#include <iostream>

#include "CoupledClusterDiagram.h"
#include "Diagram.h"

using std::cout;

void CoupledClusterDiagram::CheckVertices() {    

    // Iterate over vertices to check for virtual and H vertices
    int n_virtual = 0;
    int n_Hvertex = 0;

    for (int i=0; i<vertices.size(); ++i) {

        if (vertices[i]->IsVirtual()) {
            n_virtual++;
        }
        if (vertices[i]->IsHVertex()) {
            n_Hvertex++;
            // Move H vertex to the front if not already there
            if (i != 0) { std::swap(vertices[0], vertices[i]); }
        }
    }

    has_virtual_vertex = (n_virtual > 0);
    has_Hvertex = (n_Hvertex > 0);

    return;
}

bool CoupledClusterDiagram::GetConnectivity(const IntMat &adj) {
    
    bool connected = Diagram::GetConnectivity(adj);
    if (!connected) return false;

    // Additional checks specific to Coupled Cluster diagrams can be added here
    
    if (!has_Hvertex) return false; // Must have H vertex
    // H vertex must be connected to all other vertices
    for (int j=1; j<vertices.size(); ++j) {
        if (adjacency_matrix(0,j) + adjacency_matrix(j,0)==0 ) return false;
    }

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