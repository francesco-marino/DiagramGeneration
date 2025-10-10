#include "Diagram.h"


void Diagram::BuildFromAdjacencyMatrix(const IntMat& mat) {
    adjacency_matrix = mat;
    this->ProcessMatrix();
} 

void Diagram::Cleanup() {
    vertices.clear();
    adjacency_matrix.clear();
    is_connected = false;
}


// TOOD HERE Implement this function
void Diagram::ProcessMatrix() {
}



        