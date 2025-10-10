#pragma once

#include <armadillo>
#include <vector>

#include "Vertex.h"

using std::vector;
typedef arma::Mat<int> IntMat;
typedef arma::Row<int> IntRow;



class Diagram {

    public:

        Diagram() {}
        Diagram(const IntMat& mat) { BuildFromAdjacencyMatrix(mat); }

        ~Diagram() {}

        IntMat GetAdjacencyMatrix() const { return adjacency_matrix; }
        vector<Vertex> GetVertices() const { return vertices; }
        bool IsConnected() const { return is_connected; }

        void PrintMat() const { adjacency_matrix.print(); }

    protected:

        vector<Vertex> vertices;
        IntMat adjacency_matrix;
        
        bool is_connected;

        void ProcessMatrix(); // Processes the adjacency matrix to determine connectivity and other properties
        void GetConnectivity() ; // Determines if the diagram is connected
        vector<int> GetSkeletonStructure() const = 0; // Placeholder for skeleton structure extraction

    private:

        void BuildFromAdjacencyMatrix(const IntMat& mat) ;
        void Cleanup() ;

};