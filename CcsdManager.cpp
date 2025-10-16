#include "CcsdManager.h"
#include "CoupledClusterVertex.h"
#include "Vertex.h"

void CcsdManager::Build(bool do_ccd, bool only_connected) {
    this->ConstructCcsdDiagrams(do_ccd);
    return;
}

void CcsdManager::Build(int trunc_level, bool include_3n, bool include_singles, int ntriples) {
    this->ConstructCcsdDiagrams(trunc_level, include_3n, include_singles, ntriples);
    return;
}

void CcsdManager::ConstructCcsdDiagrams(int trunc_level, bool include_3n, bool include_singles, int ntriples) {
    
    bool print = (0==rank);
    double time_start = Clock();

    int Hmax = (include_3n) ? 3 : 2;
    if (trunc_level<3) ntriples = 0;

    vector< vector<int> > AllClusters = CoupledClusterDiagramManager::ListAllClusters(trunc_level, Hmax);

    // Remove sequences with T3 when ntriples=0
    if (trunc_level>=3) {
        for (int i=0; i<AllClusters.size(); ++i) {
            if (AllClusters[i][2]>ntriples) {
                AllClusters.erase(AllClusters.begin() + i);
                --i;
            }
        }
    }
 
    vector< Vertex > HamVertices;
    if (include_singles) HamVertices.push_back(V1bVertex());
    HamVertices.push_back(V2bVertex());
    if (include_3n) HamVertices.push_back(V3bVertex());

    vector< Vertex > ExtLines;
    ExtLines.push_back( Vertex(0,0,false,"0") );
    if (include_singles) ExtLines.push_back(T1Vertex(true,"S"));
    ExtLines.push_back( T2Vertex(true,"D") );
    if (trunc_level>=3 && ntriples>0) ExtLines.push_back(Vertex(3,3,true,"T"));

    vector< Vertex > Vertices;
    for (int i_ext=0; i_ext<ExtLines.size(); ++i_ext) {

        for (int i_ham=0; i_ham<HamVertices.size(); ++i_ham) {

            for (int i_clust=0; i_clust<AllClusters.size(); ++i_clust) {

                Vertices.clear();
                if (ExtLines[i_ext].GetNout() > 0) Vertices.push_back(ExtLines[i_ext]); // Do not include the empty external line
                Vertices.push_back(HamVertices[i_ham]);

                auto &seq = AllClusters[i_clust];   // Current sequence: { n(t1), n(t2), n(t3)... }
                if (!include_singles && seq[0]>0)  continue;

                for (int j=0; j<seq.size(); ++j) {  // Iterate over n(T_j).
                    // Add vertices of correct type and multiplicty
                    for (int k=0; k<seq[j]; ++k) {
                        if (j==0) Vertices.push_back(T1Vertex());
                        if (j==1) Vertices.push_back(T2Vertex());
                        if (j==2 && seq[j]<ntriples) Vertices.push_back(T3Vertex());
                    }
                }

                this->ResetVertices();
                this->AssignVertices(Vertices);
                this->CoupledClusterDiagramManager::Build(true);

            }   //

        }   // Hamiltonian
 
    }   // External indeces

    double time_end = Clock();
    if (print) {
        std::cout << "CCSD Manager: Generated " << this->GetNumberOfDiagrams() << " diagrams in " << time_end - time_start << " seconds.\n";
    }
}

void CcsdManager::ConstructCcsdDiagrams(bool do_ccd) {
    this->ConstructCcsdDiagrams(2, false, !do_ccd, 0);
}