#include "MbptDiagram.h"

vector<int> MbptDiagram::FindSkeletonStructure(const IntMat& mat) const {
    // Follow Shavitt&Bartlett, Sec. 5.7.2
    int row = 0;
    int n = mat.n_rows;
    vector<int> skeleton;

    for (int col=0; col<n; ++col) {
        if (col == row) continue;
        int conn = mat(row, col) + mat(col, row);
        skeleton.push_back(conn);
    }

    return skeleton;
}

// TODO HERE This still needs work
bool MbptDiagram::IsConjugateToDiag(const MbptDiagram& other) { 
    bool check_adjacency = arma::all(arma::vectorise(adjacency_matrix.t() == other.adjacency_matrix));
    bool check_vertices = true;

    has_conjugate = has_conjugate || check_adjacency;
    return has_conjugate;
}


void MbptDiagram::Print() const {
    Diagram::Print();
    if (IsSelfConjugate()) {
        std::cout << "Self-conjugate.\n";
    } else if (HasConjugate()) {
        std::cout << "Has pair-conjugate.\n";
    }
    return;
}
        