#pragma once

#include <memory>

#include "DeterministicDiagram.h"
#include "DiagramManager.h"
#include "LabeledDiagram.h"
#include "SpBasis.h"

using std::cout;
using std::unique_ptr;
using std::shared_ptr;

class ComputableVertex : public VertexWithLine {
    
    public:

        ComputableVertex();
        ComputableVertex(int Nin, int Nout, bool virtual_flag=false, const std::string& name="", bool is_Hvertex=false, const std::string& latex_name="x");

        void BuildFromVertex(const unique_ptr<VertexWithLine>& base);
        
        void SetSpBasis(const shared_ptr<SpBasis> &spbasis_in) { InitBasis(spbasis_in); }
        bool IsBasisSet() const { return (spbasis!=nullptr); }

        bool CheckNonZero(vector<int> indeces) const;
        virtual bool CheckNonZero(vector<int> bra, vector<int> ket) const;

        Num Evaluate(vector<int> indeces) const;
        virtual Num Evaluate(vector<int> bra, vector<int> ket) const;

        virtual void Cleanup() override;

    protected:
        shared_ptr<SpBasis> spbasis;
        virtual void InitBasis(const shared_ptr<SpBasis> &spbasis_in);

    private:
        int nstates, nholes;
};


class DeterministicDiagram : public LabeledDiagram {
    
    public:

        DeterministicDiagram();
        DeterministicDiagram(const IntMat& mat);
        DeterministicDiagram(const IntMat& mat, const vector<Vertex>& vertices_in);
        DeterministicDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in);

        void SetSpBasis(shared_ptr<SpBasis> &spbasis_in) { Init(spbasis_in); }
        bool IsBasisSet() const { return (spbasis!=nullptr); }

        Num Compute();

        virtual void Cleanup() override;

    protected:
        shared_ptr<SpBasis> spbasis;
        vector< unique_ptr<ComputableVertex> > computable_vertices;

        Num EvalOneSample(vector<int> sp_indeces, bool sym_factor=true, bool permute=false) const;
        virtual void Init(const shared_ptr<SpBasis> &spbasis_in);

        bool AreThereEquivalentIndeces(const vector<int> indeces) const;

    private:
        int nstates, nholes;
        vector<int> p_range, h_range;

};


class DeterministicDiagramManager : public DiagramManager {
    public:
        virtual void SetSpBasis(shared_ptr<SpBasis> &spbasis_in);
        void SetSpBasis(unique_ptr<SpBasis> &spbasis_in);

        virtual vector<Num> Compute();

    protected:

        std::shared_ptr<SpBasis> spbasis;

        void Cleanup() override;
    
};