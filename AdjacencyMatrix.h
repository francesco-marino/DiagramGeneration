#pragma once

#include <armadillo>
#include <iostream>
#include <set>
#include <vector>

#include "DataStructures.h"

using namespace arma;
using std::vector;
using std::set;

// Define square matrix of integers
typedef arma::Mat<int> IntMat;
typedef arma::Row<int> IntRow;

vector<IntMat> generate_adjacency_matrices(int n, int sum=2);

class AdjacencyMatrix {
    
    public:
        AdjacencyMatrix(const IntMat& matrix) : mat(matrix), n(matrix.n_rows) {}

        vector<int> SkeletonStructure() const;
        int CountLoops() const;

        void Print() const { mat.print(); }
    
    private:
        IntMat mat;
        int n;
};


class MbptDiagramsManager {
    // Manages the generation and storage of MBPT diagrams using adjacency matrices

    public:
        MbptDiagramsManager() { Clear(); };
        MbptDiagramsManager(const vector<AdjacencyMatrix> & matrices) 
            : adjacency_matrices(matrices) { Clear(); ProcessDiagrams(); }

        void GenerateDiagrams(int n, int sum=2);
        MbptDiagramsManager SelectBySkeletonStructure(const vector<int>& structure);

        vector<IntMat> GetAdjacencyIntMat() const { return adjacency_intmat; }
        vector<AdjacencyMatrix> GetAdjacencyMatrices() const { return adjacency_matrices; }
        vector< vector<int> > FindSkeletonStructures() const { return skeleton_structures; }

        set< vector<int> > GetUniqueSkeletonStructures() const {  
            set< vector<int> > Set(skeleton_structures.begin(), skeleton_structures.end()); 
            return Set; 
        }
    
        void PrintMatrices() const;

    private:
        vector<IntMat> adjacency_intmat;
        vector<AdjacencyMatrix> adjacency_matrices;
        vector< vector<int> > skeleton_structures;
        set< vector<int> > unique_skeleton_structures;

        void ProcessDiagrams();

        void FindSkeletonStructures();
        void SortBySkeletonStructure(); 

        void Clear() { adjacency_intmat.clear(); adjacency_matrices.clear(); }
};