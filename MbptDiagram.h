#pragma once

#include "Diagram.h"

class MbptDiagram : public Diagram {
    public:
        MbptDiagram() : Diagram() { type = "MBPT"; }
        MbptDiagram(const IntMat& mat) : Diagram(mat) { this->type = "MBPT"; }
        MbptDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) : Diagram(mat, vertices_in) { this->type = "MBPT"; }
        MbptDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) : Diagram(mat, std::move(vertices_in)) { this->type = "MBPT"; }
        ~MbptDiagram() {}

    protected:
        void CheckVertices() override {};
        vector<int> FindSkeletonStructure(const IntMat& mat) const override; // Implements skeleton structure extraction for MBPT diagrams
};