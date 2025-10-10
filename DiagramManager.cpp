#include <iostream>
#include <vector>

#include "DataStructures.h"
#include "DiagramManager.h"

using namespace arma;

void DiagramManager::Build(bool only_connected) {
    this->GenerateAdjacencyMatrix(only_connected);
}

void DiagramManager::AddVertex(const Vertex& vertex) {
    vertices.push_back(vertex);
}

void DiagramManager::GenerateAdjacencyMatrix(bool only_connected) {
    
    int n = vertices.size();

    // Sum( Row(i) ) = Nout(i)
    // Sum( Col(j) ) = Nin(j)

    // The first row acts as a pivot
    vector< vector<int> > sequences = generate_sequences(n, vertices[0].GetNout());
    vector<int> pivot_indices = select_by_condition(sequences, {0}, {0});

    // Generate sequences for each column
    vector< vector< vector<int> > > all_columns_sequences;
    for (int j=0; j<n; ++j) {
        vector< vector<int> > col_seq = generate_sequences(n, vertices[j].GetNin());
        all_columns_sequences.push_back(col_seq);
    }

    // The algorithm first iterates over all possible first rows (pivots).
    // For each first row, it generates all possible combinations of columns
    // that satisfy the conditions imposed by the first row.
    // Each combination is then checked to ensure that all row and column sums
    // match the required Nin and Nout values for each vertex.
    for (auto pivot_ind: pivot_indices) {

        vector<int> first_row = sequences[pivot_ind];
        IntMat adj_matrix = IntMat(n, n, fill::zeros);
        bool empty_col = false;

        // Fix the first row (pivot)
        adj_matrix.row(0) = conv_to<arma::Row<int>>::from(first_row);

        vector< vector<int> > all_columns;
        for (int j=0; j<n; ++j) {
            vector<int> col_indices = select_by_condition(all_columns_sequences[j], {0, j}, { first_row[j], 0 } );
            if (col_indices.size() == 0) { empty_col = true; break; }
            all_columns.push_back(col_indices);
        }
        if (empty_col) { continue; }

        // Generate all combinations of columns by Cartesian product
        vector< vector<int> > column_combinations = cartesian_product(all_columns);

        for (const auto& col_comb: column_combinations) {

            IntMat candidate_matrix = adj_matrix;
            for (int j=0; j<n; ++j) {
                int col_index = col_comb[j];
                candidate_matrix.col(j) = conv_to<arma::Col<int>>::from( all_columns_sequences[j][col_index] );
            }
            
            // Check that sum of each row and column is equal to 'sum'
            bool valid = true;
            for (int i=0; i<n; ++i) {
                if (arma::accu(candidate_matrix.row(i)) != vertices[i].GetNout() || arma::accu(candidate_matrix.col(i)) != vertices[i].GetNin()) {
                    valid = false;
                    break;
                }
            }

            // Convert into Diagram
            auto diag = CreateDiagram(candidate_matrix);
            if (only_connected && !diag->IsConnected()) { valid = false; }

            if (valid) {
                diagrams.push_back(*diag);
                adjacency_matrices.push_back(candidate_matrix);
            }
        }
    }

    this->adjacency_matrices = adjacency_matrices;
    return;
}


void DiagramManager::GenerateDiagramsFromMatrices(const vector<IntMat>& matrices) {
    diagrams.clear();
    for (const auto& mat : matrices) {
        Diagram diag(mat);
        diagrams.push_back(diag);
    }
}


void DiagramManager::PrintMatrices() const {

    // Iterate over diagrams to print their adjacency matrices
    for (int ind=0; ind<diagrams.size(); ++ind) {
        std::cout << "Matrix " << ind+1 << ":\n";
        diagrams[ind].PrintMat();
        std::cout << "\n";
    }

    return;
}




void DiagramManager::Print() const {
    // Print details of each diagram
    if (0==diagrams.size()) {
        std::cout << "No diagrams to display.\n";
        return;
    }
    for (int ind=0; ind<diagrams.size(); ++ind) {
        std::cout << "Diagram " << ind+1 << ":\n";
        diagrams[ind].Print();
        std::cout << "\n";
    }
    return;
}

void DiagramManager::Cleanup() {
    vertices.clear();
   
    for (auto& mat : adjacency_matrices) { mat.clear(); }
    adjacency_matrices.clear();

    for (auto& diag : diagrams) { /* diag.Cleanup(); */ } // Assuming Diagram has a Cleanup method
    diagrams.clear();
}


void DiagramManager::Test(int order) {
    // Try generating Mbpt diagrams of a given order
    std::cout << "Testing DiagramManager class...\n";

    for (int i=0; i<order; ++i) {
        Vertex v(2, 2);
        this->AddVertex(v);
    }
    this->Build();
    this->Print();
    return;
}