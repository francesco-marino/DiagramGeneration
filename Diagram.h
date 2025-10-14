#pragma once

#include <armadillo>
#include <memory>
#include <string>
#include <vector>

#include <boost/graph/adjacency_matrix.hpp>

#include "Graph.h"
#include "Vertex.h"

using std::vector;
using std::string;
using std::unique_ptr;

typedef arma::Mat<int> IntMat;
typedef arma::Row<int> IntRow;


class Diagram {

    public:

        Diagram() { InitDiagram(); }
        Diagram(const IntMat& mat) { InitDiagram(); adjacency_matrix = mat; }
        Diagram(const IntMat& mat, const vector<Vertex>& vertices_in);
        Diagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in);

        ~Diagram() { Cleanup();}

        // The strategy is to have a fast Build method. If a diagram is valid, we can then process it.
        void Build() { this->Build(adjacency_matrix); }
        void Build(const IntMat& mat) { BuildFromAdjacencyMatrix(mat); built = true;}

        virtual void Process();

        IntMat GetAdjacencyMatrix() const { return adjacency_matrix; }

        void SetVertices(const vector< unique_ptr<Vertex> >& vertices_in);
        void SetVertices(const vector<Vertex>& vertices_in);
        const vector< unique_ptr<Vertex> >& GetVertices() const { return vertices; }

        bool HasHVertex() const { return has_Hvertex; }
        bool HasVirtualVertex() const { return has_virtual_vertex; }
        bool IsConnected() const { return is_connected; }
        bool IsBuilt() const { return built; }
        vector<int> GetSkeletonStructure() const { return skeleton_structure; }

        void SetType(const string& type) { this->type = type; }
        string GetType() const { return type; }

        void PrintMat() const { adjacency_matrix.print(); }
        virtual void Print() const;

        void PrintGraph() const { directed_graph->Print(); }

        virtual void Cleanup() ;

        void BuildDirectedGraph();

    protected:

        vector< unique_ptr<Vertex> > vertices;
        IntMat adjacency_matrix;
        vector<int> skeleton_structure;
        unique_ptr<DirectedGraph> directed_graph;
        vector< vector<int> > equivalent_vertices;
        
        bool is_connected;
        bool has_virtual_vertex;
        bool has_Hvertex;

        int pos_Hvertex;
        int pos_virtual_vertex;
        
        string type;
        bool built;

        virtual void CheckVertices() {};
        void GetConnectivity(); // Determines if the diagram is connected
        virtual bool GetConnectivity(const IntMat &adj);

        void FindSkeletonStructure() { skeleton_structure = FindSkeletonStructure(adjacency_matrix);  }
        virtual vector<int> FindSkeletonStructure(const IntMat& mat) const { return {13,12}; }; // Placeholder for skeleton structure extraction

        virtual vector< vector<int> > FindEquivalentVertices(const vector< unique_ptr<Vertex> >& vertices_in); // Placeholder for finding equivalent vertices

    private:

        unique_ptr<DirectedGraph> BuildDirectedGraph(const IntMat& mat) const;
        
        void BuildFromAdjacencyMatrix(const IntMat& mat);
        void InitDiagram() { Cleanup(); type = "Generic"; }
};

