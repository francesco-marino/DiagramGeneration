#pragma once

#include <armadillo>
#include <string>
#include <vector>

#include "Vertex.h"

using std::vector;
using std::string;
typedef arma::Mat<int> IntMat;
typedef arma::Row<int> IntRow;


class Diagram {

    public:

        Diagram() { InitDiagram(); }
        Diagram(const IntMat& mat) { InitDiagram(); adjacency_matrix = mat; }
        Diagram(const IntMat& mat, const vector<Vertex>& vertices_in) { InitDiagram(); adjacency_matrix = mat; SetVertices(vertices_in); }

        ~Diagram() { Cleanup();}

        void Build() { this->Build(adjacency_matrix); }
        void Build(const IntMat& mat) { BuildFromAdjacencyMatrix(mat); built = true;}

        IntMat GetAdjacencyMatrix() const { return adjacency_matrix; }
        
        void SetVertices(const vector<Vertex>& vertices_in) { vertices = vertices_in; }
        vector<Vertex> GetVertices() const { return vertices; }
        
        bool IsConnected() const { return is_connected; }
        bool IsBuilt() const { return built; }
        vector<int> GetSkeletonStructure() const { return skeleton_structure; }

        void SetType(const string& type) { this->type = type; }
        string GetType() const { return type; }

        void PrintMat() const { adjacency_matrix.print(); }
        void Print() const;

        virtual void Cleanup() ;

    protected:

        vector<Vertex> vertices;
        IntMat adjacency_matrix;
        vector<int> skeleton_structure;
        
        bool is_connected;
        bool has_virtual_vertex;
        bool has_Hvertex;
        
        string type;
        bool built;

        virtual void CheckVertices() {};
        void GetConnectivity(); // Determines if the diagram is connected
        virtual bool GetConnectivity(const IntMat &adj);

        void FindSkeletonStructure() { skeleton_structure = FindSkeletonStructure(adjacency_matrix);  }
        virtual vector<int> FindSkeletonStructure(const IntMat& mat) const { return {13,12}; }; // Placeholder for skeleton structure extraction

    private:
        void BuildFromAdjacencyMatrix(const IntMat& mat);
        void InitDiagram() { Cleanup(); type = "Generic"; }
};

class MbptDiagram : public Diagram {
    public:
        MbptDiagram() : Diagram() { type = "MBPT"; }
        MbptDiagram(const IntMat& mat) : Diagram(mat) { this->type = "MBPT"; }
        MbptDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) : Diagram(mat, vertices_in) { this->type = "MBPT"; }
        ~MbptDiagram() {}

    protected:
        void CheckVertices() override {};
        vector<int> FindSkeletonStructure(const IntMat& mat) const override; // Implements skeleton structure extraction for MBPT diagrams
};