#include <iostream>
#include <vector>

#include "AdjacencyMatrix.h"
#include "DataStructures.h"
#include "DiagramManager.h"
#include "Vertex.h"

using namespace std;


int main() {
    
    /*
    // Test adjacency matrix generation
    cout << "\nAdjacency matrices of size 4 with sum 2:\n";
    auto adj_matrices = generate_adjacency_matrices(4, 2);
    for (const auto& mat : adj_matrices) {
        mat.print();
        cout << "\n";
    }
    cout << "Total matrices generated: " << adj_matrices.size() << "\n";


    // Now test the MbptDiagramsManager class
    MbptDiagramsManager manager;
    manager.GenerateDiagrams(4, 2);
    // manager.PrintMatrices();

    for (auto skeleton : manager.GetUniqueSkeletonStructures() ) {
        cout << "Skeleton structure: ";
        for (auto val : skeleton) {
            cout << val << " ";
        }
        cout << "\n";
        MbptDiagramsManager filtered_manager = manager.SelectBySkeletonStructure(skeleton);
        cout << "Number of diagrams with this structure: " << filtered_manager.GetAdjacencyMatrices().size() << "\n\n";
        //filtered_manager.PrintMatrices();
    }
    */

    DiagramManager diagram;
    diagram.Test(6);

    return 0;
}



