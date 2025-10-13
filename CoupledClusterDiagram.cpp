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
            pos_virtual_vertex = i;
        }
        if (vertices[i]->IsHVertex()) {
            n_Hvertex++;
            pos_Hvertex = i;
        }
    }

    // Amplitudes can not be connected to each other, but just to H or the virtual vertex.
    
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
    for (int j=0; j<vertices.size(); ++j) {
        if (j == pos_Hvertex) continue; // Skip H vertex itself
        if (adj(pos_Hvertex,j) + adj(j,pos_Hvertex)==0 ) {
            return false;
        }
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