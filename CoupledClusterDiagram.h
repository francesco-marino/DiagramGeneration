#pragma once

#include "Diagram.h"

class CoupledClusterDiagram : public Diagram {
    // Represents a Coupled Cluster diagram with specific properties and methods

    public:
        CoupledClusterDiagram() : Diagram() {}
        CoupledClusterDiagram(const IntMat& mat) : Diagram(mat) { this->type = "CC"; }
        CoupledClusterDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) : Diagram(mat, vertices_in) { this->type = "CC"; }
        CoupledClusterDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) : Diagram(mat, std::move(vertices_in)) { this->type = "CC"; }
        ~CoupledClusterDiagram() {}

    protected:

        void CheckIsValid() override;
        bool GetConnectivity(const IntMat &adj) override ;
        vector<int> FindSkeletonStructure(const IntMat& mat) const override; // Implements skeleton structure extraction for CC diagrams

        // vector< vector<int> > FindEquivalentVertices(const vector< unique_ptr<Vertex> >& vertices_in) override { return Diagram::FindEquivalentVertices(vertices_in); }
};