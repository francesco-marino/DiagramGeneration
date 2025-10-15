#include <iostream>
#include <vector>

#include "AdjacencyMatrix.h"
#include "CoupledClusterDiagramManager.h"
#include "CoupledClusterVertex.h"
#include "CcsdManager.h"
#include "DataStructures.h"
#include "DiagramManager.h"
#include "LabeledDiagram.h"
#include "MbptDiagramManager.h"
#include "Parallel.h"
#include "Vertex.h"

using namespace std;


int main() {

    int ntasks, rank;
    Initialize();
    GetRank(ntasks, rank);
    
    /*
    MbptDiagramManager diagram;
    //diagram.Test(4, false);
    //cout << "Number of diagrams generated: " << diagram.GetNumberOfDiagrams() << "\n\n";

    MbptDiagramManager mbpt_w3;
    mbpt_w3.AddVertex( Vertex(3,3) );
    mbpt_w3.AddVertex( Vertex(3,3) );
    mbpt_w3.Build();
    //mbpt_w3.Print();

    CoupledClusterDiagramManager cc_0;
    cc_0.AddVertex( T2Vertex() );
    cc_0.AddVertex( V2bVertex() );
    cc_0.Build();
    //cc_0.Print();

    cc_0.ResetVertices();
    cc_0.AddVertex( T1Vertex() );
    cc_0.AddVertex( T1Vertex() );
    cc_0.AddVertex( V2bVertex() );
    cc_0.Build();
    cc_0.Print();
    

    CoupledClusterDiagramManager cc_t2;
    cc_t2.ListAllClusters(2,2);

    // First-order diagrams - T2 with V
    cc_t2.AddVertex( V2bVertex() );
    cc_t2.AddVertex( T2Vertex() );
    cc_t2.AddVertex( T2Vertex() );
    cc_t2.AddVertex( Vertex(2,2,true,"Ext") );
    //cc_t2.Build(true);
    //cc_t2.Print();
    */

    /*
    cc_t2.Reset();
    // First-order diagrams - T2 with V1b
    cc_t2.AddVertex( V1bVertex() );
    cc_t2.AddVertex( T2Vertex() );
    cc_t2.AddVertex( Vertex(1,1,true,"Ext") );
    cc_t2.Build(false);
    cc_t2.Print();
    */

    CcsdManager ccsd;
    //ccsd.Build(3, false, true, 5);
    ccsd.Build(2);
    ccsd.SummarizeDiagrams();
    cout << ccsd.GetNumberOfDiagrams() << endl;
    //ccsd.Print();

    unique_ptr<Diagram> & diag = ccsd.GetDiagram(10);
    CoupledClusterDiagram label(diag->GetAdjacencyMatrix(), diag->GetVertices());
    cout << "\n\n";
    label.Build();
    label.Process();
    label.Print();
    label.PrintLines();

    Finalize();
    return 0;
}



