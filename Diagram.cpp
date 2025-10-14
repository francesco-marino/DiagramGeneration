#include "Diagram.h"

#include <armadillo>
#include <iostream>
#include <queue>

Diagram::Diagram(const IntMat& mat, const vector<Vertex>& vertices_in) { 
    InitDiagram(); 
    adjacency_matrix = mat; 
    SetVertices(vertices_in); 
}


Diagram::Diagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) { 
    InitDiagram(); 
    adjacency_matrix = mat; 
    SetVertices(vertices_in); 
}


void Diagram::BuildFromAdjacencyMatrix(const IntMat& mat) {
    adjacency_matrix = mat;
    CheckVertices();
    GetConnectivity();
} 

void Diagram::Process() {
    if (!built) return;
    FindSkeletonStructure();
    BuildDirectedGraph();
}

//
// Convert an adjacency matrix into a directed graph
//
unique_ptr<DirectedGraph> Diagram::BuildDirectedGraph(const IntMat& mat) const {

    int n = mat.n_cols;
    unique_ptr<DirectedGraph> boost_graph = std::make_unique<DirectedGraph>(n);

    for (int i=0; i<n; ++i) {
        for (int j=i+1; j<n; ++j) {
            for (int k=0; k<mat.at(i,j); ++k) {
                boost_graph->AddEdge(i, j);
            }
        } 
    }

    return boost_graph;
}


void Diagram::BuildDirectedGraph() {
    this->directed_graph = BuildDirectedGraph(adjacency_matrix);
}

vector< vector<int> > Diagram::FindEquivalentVertices(const vector< unique_ptr<Vertex> >& vertices_in) {
    
}

void Diagram::Cleanup() {
    vertices.clear();
    adjacency_matrix.clear();
    is_connected = false;
    has_virtual_vertex = false;
    has_Hvertex = false;
    pos_Hvertex = -1;
    pos_virtual_vertex = -1;
    built = false;
    skeleton_structure.clear();
    directed_graph.reset();
    type = "Generic";
    return;
}

void Diagram::SetVertices(const vector< unique_ptr<Vertex> >& vertices_in) {
    vertices.clear();
    for (const auto& v : vertices_in) {
        vertices.push_back(std::make_unique<Vertex>(*v));
    }
}

void Diagram::SetVertices(const vector<Vertex>& vertices_in) {
    vertices.clear();
    for (const auto& v : vertices_in) {
        vertices.push_back(std::make_unique<Vertex>(v));
    }
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

