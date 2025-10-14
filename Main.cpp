#include <iostream>
#include <vector>

#include "AdjacencyMatrix.h"
#include "CoupledClusterDiagramManager.h"
#include "CoupledClusterVertex.h"
#include "DataStructures.h"
#include "DiagramManager.h"
#include "MbptDiagramManager.h"
#include "Parallel.h"
#include "Vertex.h"

using namespace std;


int main() {

    int ntasks, rank;
    Initialize();
    GetRank(ntasks, rank);
    
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
    cc_0.Print();

    cc_0.Reset();
    cc_0.AddVertex( T1Vertex() );
    cc_0.AddVertex( T1Vertex() );
    cc_0.AddVertex( V2bVertex() );
    cc_0.Build();
    cc_0.Print();
    
    
    CoupledClusterDiagramManager cc_t2;
    cc_t2.AddVertex( T2Vertex() );
    cc_t2.AddVertex( T2Vertex() );
    cc_t2.AddVertex( V2bVertex() );
    cc_t2.AddVertex( Vertex(2,2,true,"Ext") );
    cc_t2.Build(true);
    cc_t2.Print();

    cout << "\n\n\n";
    MbptDiagramManager eom;
    eom.AddVertex( Vertex(2,2,true,"D") );
    eom.AddVertex( Vertex(1,1,false,"R") );
    eom.AddVertex( Vertex(2,2,false,"H") );
    //eom.Build(false);
    //eom.Print();

    Finalize();
    return 0;
}



