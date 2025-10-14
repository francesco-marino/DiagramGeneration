#include <iostream>
#include <vector>

#include "CoupledClusterDiagramManager.h"
#include "DataStructures.h"

using std::cout;

vector< vector<int> > CoupledClusterDiagramManager::ListAllClusters(int exc_level_max, int Hexc_level_max) {

    int order_max = exc_level_max + Hexc_level_max;

    vector< vector<int> > AllSequences;
    AllSequences.push_back( vector<int>(exc_level_max, 0) ); // Add the empty sequence

    // The algorithm iterates over the order in the expansion,
    // i.e. the n. of cluster operators, from 1 to order_max (=4 for ccsd with 2b interactions; =6 for ccsdt-1).
    // For each order, sequences of length exc_level_max are generated, to be interpreted as {n. T1, n. T2, n. T3, ...}.
    // such that n(T1)+n(T2)+... = order.
    // Then, we check the excitation level of each sequence, i.e.
    // n(T1)*1 + n(T2)*2 + n(T3)*3 + ... <= exc_level_max + Hexc_level_max,
    // and remove elements that do not satisfy this condition.
    for (int order=1; order<=order_max; ++order) {
        
        vector< vector<int> > sequences = generate_sequences(exc_level_max, order);  
        
        for (int i=0; i<sequences.size(); ++i) {
            int sum = 0;
            for (int j=0; j<sequences[i].size(); ++j) {
                sum += (j+1) * sequences[i][j];
            }
            if (sum > exc_level_max+Hexc_level_max) {
                sequences.erase(sequences.begin() + i);
                --i;
            }
        }

        AllSequences.insert(AllSequences.end(), sequences.begin(), sequences.end());

    }

    return AllSequences;

}