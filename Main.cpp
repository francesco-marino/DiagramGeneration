#include <iostream>
#include <vector>

#include "AdjacencyMatrix.h"
#include "CoupledClusterDiagramManager.h"
#include "DataStructures.h"
#include "DiagramManager.h"
#include "MbptDiagramManager.h"
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


    // Now test the MbptDiagramManager class
    MbptDiagramManager manager;
    manager.GenerateDiagrams(4, 2);
    // manager.PrintMatrices();

    for (auto skeleton : manager.GetUniqueSkeletonStructures() ) {
        cout << "Skeleton structure: ";
        for (auto val : skeleton) {
            cout << val << " ";
        }
        cout << "\n";
        MbptDiagramManager filtered_manager = manager.SelectBySkeletonStructure(skeleton);
        cout << "Number of diagrams with this structure: " << filtered_manager.GetAdjacencyMatrices().size() << "\n\n";
        //filtered_manager.PrintMatrices();
    }
    */

    MbptDiagramManager diagram;
    //diagram.Test(4);

    MbptDiagramManager mbpt_w3;
    mbpt_w3.AddVertex( Vertex(3,3) );
    mbpt_w3.AddVertex( Vertex(3,3) );
    mbpt_w3.Build();
    mbpt_w3.Print();

    CoupledClusterDiagramManager cc_t2;
    cc_t2.AddVertex( Vertex(2,2,false,"T2") );
    cc_t2.AddVertex( Vertex(2,2,false,"T2") );
    cc_t2.AddVertex( Vertex(2,2,false,"H",true) );
    cc_t2.AddVertex( Vertex(2,2,true,"Ext") );
    cc_t2.Build(true);
    cc_t2.Print();

    cout << "\n\n\n";
    MbptDiagramManager eom;
    eom.AddVertex( Vertex(2,2,true,"D") );
    eom.AddVertex( Vertex(1,1,false,"R") );
    eom.AddVertex( Vertex(2,2,false,"H") );
    eom.Build(false);
    //eom.Print();

    return 0;
}



