#include <armadillo>
#include <iostream>
#include <vector>

#include "AdjacencyMatrix.h"
#include "DataStructures.h"

using namespace arma;

vector<IntMat> generate_adjacency_matrices(int n, int sum) {

    vector< vector<int> > sequences = generate_sequences(n, sum);
    vector<IntMat> adjacency_matrices;

    vector<int> pivot_indices = select_by_condition(sequences, {0}, {0});

    for (auto pivot_ind: pivot_indices) {

        vector<int> first_row = sequences[pivot_ind];
        IntMat adj_matrix = IntMat(n, n, fill::zeros);

        adj_matrix.row(0) = conv_to<arma::Row<int>>::from(first_row);

        vector< vector<int> > all_columns;
        for (int j=0; j<n; ++j) {
            vector<int> col_indices = select_by_condition(sequences, {0, j}, { first_row[j], 0 });
            all_columns.push_back(col_indices);
        }
        // Generate all combinations of columns by Cartesian product
        vector< vector<int> > column_combinations = cartesian_product(all_columns);

        for (const auto& col_comb: column_combinations) {
            IntMat candidate_matrix = adj_matrix;
            for (int j=0; j<n; ++j) {
                candidate_matrix.col(j) = conv_to<arma::Col<int>>::from(sequences[col_comb[j]]);
            }
            
            // Check that sum of each row and column is equal to 'sum'
            bool valid = true;
            for (int i=0; i<n; ++i) {
                if (arma::accu(candidate_matrix.row(i)) != sum || arma::accu(candidate_matrix.col(i)) != sum) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                adjacency_matrices.push_back(candidate_matrix);
            }
        }

    }

    return adjacency_matrices;
}




vector<int> AdjacencyMatrix::SkeletonStructure() const {
    // Follow Shavitt&Bartlett, Sec. 5.7.2
    int row = 0;
    vector<int> skeleton;

    for (int col=0; col<n; ++col) {
        if (col == row) continue;
        int conn = mat(row, col) + mat(col, row);
        skeleton.push_back(conn);
    }

    return skeleton;
}


// TODO HERE Check this function
int AdjacencyMatrix::CountLoops() const {
    // We have a loop if i is connecte to j and j is connected to i
    int count = 0;
    for (int i=0; i<n; ++i) {
        for (int j=i+1; j<n; ++j) {
            if (mat(i,j) > 0 && mat(j,i) > 0) {
                count++;
            }
        }
    }
    return count/2;
}


void MbptDiagramsManager::GenerateDiagrams(int n, int sum) {
    adjacency_intmat = generate_adjacency_matrices(n, sum);
    adjacency_matrices.clear();
    for (const auto& mat: adjacency_intmat) {
        adjacency_matrices.push_back( AdjacencyMatrix(mat) );
    }
    this->ProcessDiagrams();
}


void MbptDiagramsManager::PrintMatrices() const {

    for (int ind=0; ind<adjacency_matrices.size(); ++ind) {
        std::cout << "Matrix " << ind+1 << ":\n";
        adjacency_matrices[ind].Print();
        std::cout << "\n";
        /*
        std::cout << "Skeleton structure: ";
        for (auto val : skeleton_structures[ind]) {
            std::cout << val << " ";
        }
        std::cout << "\n\n";
        */
    }

}


void MbptDiagramsManager::FindSkeletonStructures() {
    skeleton_structures.clear();
    for (const auto& adj_mat : adjacency_matrices) {
        skeleton_structures.push_back( adj_mat.SkeletonStructure() );
    }
}


MbptDiagramsManager MbptDiagramsManager::SelectBySkeletonStructure(const vector<int>& structure) {
    
    vector<AdjacencyMatrix> selected_matrices;
    for (int i=0; i<adjacency_matrices.size(); ++i) {
        if (skeleton_structures[i] == structure) {
            selected_matrices.push_back( adjacency_matrices[i] );
        }
    }
    return MbptDiagramsManager(selected_matrices);
}


void MbptDiagramsManager::ProcessDiagrams() {
    this->FindSkeletonStructures();
    unique_skeleton_structures = this->GetUniqueSkeletonStructures();
}