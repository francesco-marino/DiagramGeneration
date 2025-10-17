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
    
    CcsdManager ccsd;
    //ccsd.Build(3, false, true, 5);
    ccsd.Build(2, false, false);
    ccsd.SummarizeDiagrams();
    cout << ccsd.GetNumberOfDiagrams() << endl;
    cout << ccsd.GetLatexExpressions(true) << endl;
    //ccsd.Print();

    unique_ptr<Diagram> & diag = ccsd.GetDiagram(0);
    CoupledClusterDiagram label(diag->GetAdjacencyMatrix(), diag->GetVertices());
    cout << "\n\n";
    label.Build();
    label.Process();
    label.Print();
    label.PrintLines();
    cout << label.GetDiagramLatexExpression() << endl;

    Finalize();
    return 0;
}



