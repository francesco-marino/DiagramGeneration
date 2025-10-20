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
#include "PairingModel.h"
#include "Parallel.h"
#include "Vertex.h"

using namespace std;


int main() {

    int ntasks, rank;
    Initialize();
    GetRank(ntasks, rank);

    bool test_diagrams = true;
    bool test_pairing_model = false;
    
    if (test_diagrams) {

        MbptDiagramManager mbpt;
        mbpt.SetMbptOrder(3);
        mbpt.Build();
        mbpt.SummarizeDiagrams();
        cout << mbpt.GetNumberOfDiagrams() << endl;
        cout << mbpt.GetLatexExpressions(true) << endl;
        cout << "\n\n\n";

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
    }

    if (test_pairing_model) {
        cout << "Testing the pairing model" << endl;
        PairingModel model;
        model.SetParams(4, 4, 1., 1.);
        model.Build();
        cout << model.IsBuilt() << endl;
        model.PrintBasis();

        double ehf = model.GetRefEnergy();
        double e2 = model.GetMbpt2();
        cout << "HF : " << ehf << endl;
        cout << "Analytical : " << model.GetMbpt2Analytical() << endl;
        cout << "Numerical : " << e2 << "  " << ehf+e2 << endl;
        double e3 = model.GetMbpt3();
        cout << "E3 : " << e2+e3 << endl;
    }

    Finalize();
    return 0;
}



