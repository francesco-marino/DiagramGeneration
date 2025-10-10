#include "Diagram.h"

#include <armadillo>
#include <queue>

void Diagram::BuildFromAdjacencyMatrix(const IntMat& mat) {
    adjacency_matrix = mat;
    this->ProcessMatrix();
} 

void Diagram::Cleanup() {
    vertices.clear();
    adjacency_matrix.clear();
    is_connected = false;
}


// TOOD HERE Implement this function
void Diagram::ProcessMatrix() {
    GetConnectivity();
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
    std::cout << "Adjacency Matrix:\n";
    adjacency_matrix.print();
    std::cout << "Is Connected: " << (is_connected ? "Yes" : "No") << "\n";
    return;
}


        