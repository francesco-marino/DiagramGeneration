#pragma once

#include "Diagram.h"
#include "DeterministicDiagram.h"

class MbptDiagram : public DeterministicDiagram {
    public:
        MbptDiagram() : DeterministicDiagram() { InitMbptDiagram(); }
        MbptDiagram(const IntMat& mat) : DeterministicDiagram(mat) { InitMbptDiagram(); }
        MbptDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) : DeterministicDiagram(mat, vertices_in) { InitMbptDiagram(); }
        MbptDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) : DeterministicDiagram(mat, std::move(vertices_in)) { InitMbptDiagram(); }
        ~MbptDiagram() {}

        virtual void Process();

        bool IsSelfConjugate() const { return GetAdjacencyMatrix().is_symmetric(); }
        bool HasConjugate() const { return has_conjugate; }
        bool IsConjugateToDiag(const MbptDiagram& other) ;

        void Print() const override;
        virtual string GetDiagramLatexExpression(bool show_ext=false) const;

    protected:

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