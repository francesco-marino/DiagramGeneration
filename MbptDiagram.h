#pragma once

#include "Diagram.h"

class MbptDiagram : public Diagram {
    public:
        MbptDiagram() : Diagram() { InitMbptDiagram(); }
        MbptDiagram(const IntMat& mat) : Diagram(mat) { InitMbptDiagram(); }
        MbptDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) : Diagram(mat, vertices_in) { InitMbptDiagram(); }
        MbptDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) : Diagram(mat, std::move(vertices_in)) { InitMbptDiagram(); }
        ~MbptDiagram() {}

        bool IsSelfConjugate() const { return adjacency_matrix.is_symmetric(); }
        bool HasConjugate() const { return has_conjugate; }
        bool IsConjugateToDiag(const MbptDiagram& other) ;

        void Print() const override;

    protected:
        void CheckVertices() override {};
        vector<int> FindSkeletonStructure(const IntMat& mat) const override; // Implements skeleton structure extraction for MBPT diagrams

    private:
        bool is_self_conjugate;
        bool has_conjugate;

        void InitMbptDiagram() { type = "MBPT"; is_self_conjugate = false; has_conjugate = false; }
};