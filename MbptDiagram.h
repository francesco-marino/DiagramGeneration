#pragma once

#include "Diagram.h"
#include "LabeledDiagram.h"

class MbptDiagram : public LabeledDiagram {
    public:
        MbptDiagram() : LabeledDiagram() { InitMbptDiagram(); }
        MbptDiagram(const IntMat& mat) : LabeledDiagram(mat) { InitMbptDiagram(); }
        MbptDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) : LabeledDiagram(mat, vertices_in) { InitMbptDiagram(); }
        MbptDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) : LabeledDiagram(mat, std::move(vertices_in)) { InitMbptDiagram(); }
        ~MbptDiagram() {}

        virtual void Process();

        bool IsSelfConjugate() const { return GetAdjacencyMatrix().is_symmetric(); }
        bool HasConjugate() const { return has_conjugate; }
        bool IsConjugateToDiag(const MbptDiagram& other) ;

        virtual void Cleanup();
        void Print() const override;
        virtual string GetDiagramLatexExpression(bool show_ext=false) const;

    protected:

        vector< unique_ptr<VertexWithLine> > energy_denoms;

        void CheckIsValid() override;
        vector<int> FindSkeletonStructure(const IntMat& mat) const override; // Implements skeleton structure extraction for MBPT diagrams

        void FindLineType() override;
        void AssignNamesToLines() override;
        void FindDiagramExpression() override;

    private:
        bool is_self_conjugate;
        bool has_conjugate;

        void InitMbptDiagram() { this->type = "MBPT"; is_self_conjugate = false; has_conjugate = false; }
        void FindEnergyDenoms();
};