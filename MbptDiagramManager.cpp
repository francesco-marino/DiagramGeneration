#include <unordered_set>

#include "MbptDiagramManager.h"

using std::unordered_set;
using std::cout;

void MbptDiagramManager::PostProcessDiagrams() {

    // Devise an algorithm that iterates over all the diagrams.
    // They must be interpreted as MBPT diagrams, so we can check for self-conjugacy.
    // If they are self-conjugate, we mark them as such.
    // If they are not, we search for their conjugate counterpart in the list.
    // The conjugate diagram is then removed from the list, and the original diagram is marked as having a conjugate.
    unordered_set<int> to_remove;
    for (int i=0; i<diagrams.size(); ++i) {
        if (to_remove.find(i) != to_remove.end() ) continue; // Already marked for removal

        MbptDiagram* diag = dynamic_cast<MbptDiagram*>(diagrams[i].get());
        if (diag->IsSelfConjugate()) {
            // Mark as self-conjugate
            continue;
        } else {
            // Search for conjugate
            for (int j=i+1; j<diagrams.size(); ++j) {
                if (to_remove.find(j) != to_remove.end() ) continue; // Already marked for removal

                MbptDiagram* other_diag = dynamic_cast<MbptDiagram*>(diagrams[j].get());
                if (diag->IsConjugateToDiag(*other_diag)) {
                    // Found conjugate
                    to_remove.insert(j);
                    // cout << "Diagram " << i+1 << " is conjugate to Diagram " << j+1 << ". Removing Diagram " << j+1 << ".\n";
                    break;
                }
            }
        }
    }

    // Remove marked diagrams in reverse order to avoid invalidating indices
    vector< unique_ptr<Diagram> > filtered_diagrams;
    for (int i=0; i<diagrams.size(); ++i) {
        if (to_remove.find(i) == to_remove.end() ) {
            filtered_diagrams.push_back(std::move(diagrams[i]));
        }
    }
    diagrams = std::move(filtered_diagrams);
    // Rebuild adjacency_matrices
    adjacency_matrices.clear();
    for (const auto& diag : diagrams) { 
        this->adjacency_matrices.push_back(diag->GetAdjacencyMatrix());
    }

    return;
}
