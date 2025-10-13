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

        