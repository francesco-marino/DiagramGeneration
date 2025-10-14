#pragma once

#include <armadillo>
#include <memory>
#include <vector>

#include "DataStructures.h"
#include "Diagram.h"
#include "Parallel.h"
#include "Vertex.h"

using std::vector; 
using std::string;
using std::unique_ptr;
using std::make_unique;

class DiagramManager {

    public:

        DiagramManager() { Cleanup(); GetRank(ntasks, rank); }
        DiagramManager(const vector<Vertex>& vertices);
        ~DiagramManager() { Cleanup(); }

        void AddVertex(const unique_ptr<Vertex>& vertex);
        void AddVertex(const Vertex& vertex);

        void Build(bool only_connected=false);

        void PrintMatrices() const;
        void Print() const;
        void Reset() { Cleanup(); }

        vector<IntMat> GetAdjacencyMatrices() const { return adjacency_matrices; }
        vector< unique_ptr<Diagram> >& GetDiagrams() { return diagrams; }
        unique_ptr<Diagram>& GetDiagram(int index) { return diagrams[index]; }
        vector< unique_ptr<Vertex> >& GetVertices() { return vertices; }

        int GetNumberOfDiagrams() const { return diagrams.size(); }

        void Test(int order, bool print=true);

    protected:

        int rank, ntasks;

        vector< unique_ptr<Vertex> > vertices;
        vector< unique_ptr<Diagram> > diagrams;
        vector<IntMat> adjacency_matrices;

        // Factory method to create Diagram instances
        unique_ptr<Diagram> CreateDiagram(const IntMat& mat) const { return CreateDiagram(mat, vector<Vertex>()); }
        virtual unique_ptr<Diagram> CreateDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) const = 0;
        virtual unique_ptr<Diagram> CreateDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) const = 0;

        virtual void PostProcessDiagrams() = 0; // TODO HERE Change name

        void Cleanup();
       
     private:

        void AssignVertices(const vector<Vertex>& vertices_in);
        void AssignVertices(const vector< unique_ptr<Vertex> >& vertices_in);

        void EnumerateDiagrams(bool only_connected=false);

};