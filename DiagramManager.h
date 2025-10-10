#pragma once

#include <armadillo>
#include <vector>

#include "DataStructures.h"
#include "Diagram.h"
#include "Vertex.h"

using std::vector; 

class DiagramManager{

    public:

        DiagramManager() { Cleanup(); }
        DiagramManager(const vector<Vertex>& vertices);
        ~DiagramManager() { Cleanup(); }

        void AddVertex(const Vertex& vertex);
        void Build();

        void PrintMatrices() const;

        vector<IntMat> GetAdjacencyMatrices() const { return adjacency_matrices; }
        vector<Diagram> GetDiagrams() const { return diagrams; }
        Diagram GetDiagram(int index) const { return diagrams[index]; }
        vector<Vertex> GetVertices() const { return vertices; }

        void Test(int order);

    protected:
        vector<Vertex> vertices;
        vector<Diagram> diagrams;
        vector<IntMat> adjacency_matrices;

        void AssignVertices(const vector<Vertex>& vertices_in) { vertices = vertices_in; }

    private:
        void Cleanup();
        void GenerateAdjacencyMatrix();
        void GenerateDiagramsFromMatrices(const vector<IntMat>& matrices);
        void PrintMat(const IntMat& mat) const { mat.print(); }
};