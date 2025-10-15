#pragma once

#include "LabeledDiagram.h"

class CoupledClusterDiagram : public LabeledDiagram {
    // Represents a Coupled Cluster diagram with specific properties and methods

    public:
        CoupledClusterDiagram() : LabeledDiagram() {}
        CoupledClusterDiagram(const IntMat& mat) : LabeledDiagram(mat) { this->type = "CC"; }
        CoupledClusterDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) : LabeledDiagram(mat, vertices_in) { this->type = "CC"; }
        CoupledClusterDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) : LabeledDiagram(mat, std::move(vertices_in)) { this->type = "CC"; }
        ~CoupledClusterDiagram() {}

        std::string GetVertexString() const override;
        
    protected:

        void CheckIsValid() override;
        bool GetConnectivity(const IntMat &adj) override ;
        vector<int> FindSkeletonStructure(const IntMat& mat) const override; // Implements skeleton structure extraction for CC diagrams

        void FindLineType() override;
        // vector< vector<int> > FindEquivalentVertices(const vector< unique_ptr<Vertex> >& vertices_in) override { return LabeledDiagram::FindEquivalentVertices(vertices_in); }
};