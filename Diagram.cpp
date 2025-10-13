#include "Diagram.h"

#include <armadillo>
#include <queue>

void Diagram::BuildFromAdjacencyMatrix(const IntMat& mat) {
    adjacency_matrix = mat;
    CheckVertices();
    GetConnectivity();
    FindSkeletonStructure();
} 

void Diagram::Cleanup() {
    vertices.clear();
    adjacency_matrix.clear();
    is_connected = false;
    has_virtual_vertex = false;
    has_Hvertex = false;
    built = false;
    skeleton_structure.clear();
    type = "Generic";
    return;
}


bool Diagram::GetConnectivity(const IntMat &adj)  {
    
    int n = adj.n_rows;
    if (n == 0) return true; // empty graph = trivially connected

    std::vector<bool> visited(n, false);
    std::queue<int> q;

    // Start BFS from node 0
    visited[0] = true;
    q.push(0);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int u = 0; u < n; ++u) {
            if (adj(v, u) && !visited[u]) {
                visited[u] = true;
                q.push(u);
            }
        }
    }

    // Check if all vertices were visited
    for (bool v : visited)
        if (!v) return false;

    return true;
}


void Diagram::GetConnectivity()  {
    is_connected = GetConnectivity(adjacency_matrix);
    return;
}

void Diagram::Print() const {
    std::cout << "Diagram Type: " << GetType() << "\n";
    std::cout << "Is Connected: " << (is_connected ? "Yes" : "No") << "\n";
    std::cout << "Adjacency Matrix:\n";
    adjacency_matrix.print();
    std::cout << "Skeleton Structure: ";
    for (auto val : skeleton_structure) { std::cout << val << " "; }
    std::cout << "\n";
    return;
}


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

        