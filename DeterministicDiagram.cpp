
#include  <iostream>

#include "DeterministicDiagram.h"

using std::make_unique;
using std::cout;
using std::endl;


ComputableVertex::ComputableVertex() : VertexWithLine() { }

ComputableVertex::ComputableVertex(int Nin, int Nout, bool virtual_flag, const std::string& name, bool is_Hvertex, const std::string& latex_name) : 
    VertexWithLine(Nin, Nout, virtual_flag, name, is_Hvertex, latex_name) {}


void ComputableVertex::BuildFromVertex(const unique_ptr<VertexWithLine>& base) {
    this->Init(base->GetNin(), base->GetNout(), 
        base->IsVirtual(), base->IsHVertex(),
        base->GetName(), base->GetLatexSymbol() );

    // Copy indeces
    for (int ind : base->GetOutLineIndeces() ) this->AddLineIndex(ind, true);
    for (int ind : base->GetInLineIndeces() )  this->AddLineIndex(ind, false);
    this->Nin  = in_line_ind.size();
    this->Nout = out_line_ind.size();

    return;
}


void ComputableVertex::InitBasis(const shared_ptr<SpBasis> &spbasis_in) {
    spbasis = spbasis_in;
    nholes  = spbasis->GetNumberOfHoles();
    nstates = spbasis->GetNumberOfStates();
}

void ComputableVertex::Cleanup() {
    VertexWithLine::Cleanup();
    spbasis = nullptr;
}

Num ComputableVertex::Evaluate(vector<int> indeces) const {
    if ( indeces.size()!=Nout+Nin ) return 0.;
    vector<int> bra(indeces.begin(), indeces.begin()+Nout);
    vector<int> ket(indeces.begin()+Nout, indeces.end());
    return Evaluate(bra, ket);
}

Num ComputableVertex::Evaluate(vector<int> bra, vector<int> ket) const {

    Num res = 0.;
    int nbra = this->GetNout();
    int nket = this->GetNin();
    if (bra.size()!=nbra || ket.size()!=nket) return 0.;

    if (nbra==2 && nket==2) {
        res = spbasis->ComputeV(bra[0], bra[1], ket[0], ket[1]);
    }
    else if (nbra==1 && nket==1) {
        res = spbasis->ComputeF(bra[0], ket[0]);
    }

    return res;
}


std::vector<int> range(int start, int end, int step = 1) {
    std::vector<int> result;
    for (int i = start; (step > 0) ? i < end : i > end; i += step) {
        result.push_back(i);
    }
    return result;
}

DeterministicDiagram::DeterministicDiagram() {
    Diagram();
}

DeterministicDiagram::DeterministicDiagram(const IntMat& mat) : LabeledDiagram(mat) { 
}

DeterministicDiagram::DeterministicDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) : LabeledDiagram(mat, vertices_in) { 
}

DeterministicDiagram::DeterministicDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) : 
    LabeledDiagram(mat, vertices_in) { 
}


void DeterministicDiagram::Init(const shared_ptr<SpBasis> &spbasis_in) {
    spbasis = spbasis_in;
    if ( !spbasis->IsBuilt() ) spbasis->Build();
    nholes  = spbasis->GetNumberOfHoles();
    nstates = spbasis->GetNumberOfStates();

}

Num DeterministicDiagram::Compute() {

    if (!IsBuilt()) return -1;

    int nlines = lines.size();
    int nvertices = v_with_lines.size();

    int nh_lines = GetNumberOfHoleLines();
    int np_lines = nlines - nh_lines;

    Num result = 0.;
    vector<int> starts(nlines, 0), ends(nlines, 0), indices(nlines+1, 0);

    for (int iv=0; iv<nvertices; ++iv) {
        unique_ptr<ComputableVertex> vert = make_unique<ComputableVertex>();
        vert->BuildFromVertex( v_with_lines[iv] );
        vert->SetSpBasis( spbasis );
        computable_vertices.push_back( std::move(vert) );
    }


    for (int il=0; il<nlines; ++il) {

        auto &line = lines[il];
        if (line->GetLineType()=="h") {
            starts[il] = 0;
            ends[il]   = nholes;
        }
        else if (line->GetLineType()=="p") {
            starts[il] = nholes;
            ends[il]   = nstates;
        }

    }

    for (int il=0; il<nlines; ++il) { indices[il] = starts[il]; }
    indices[nlines] = 0;

    // Adpated from a clever algorithm found in https://stackoverflow.com/questions/9555864/variable-nested-for-loops.
    int p = 0;
    while( indices[nlines]==0 ) {

        vector<int> slice(indices.begin(), indices.begin() + nlines);
        Num tmp = EvalOneSample(slice, true, false);
        result += tmp;

        //for (auto val : slice) { cout << val << " "; }  cout << endl;

        indices[0]++;
        while ( indices[p]==ends[p] ) {
            indices[p] = starts[p];     // Reset index that has reached its max. value
            indices[++p]++;             // Update the following index and increase p
            // This operation is iterated for all indeces that have reached the end.
            if ( indices[p]!=ends[p] ) {
                p = 0;
            }
        } 

    }

    return result;
}



Num DeterministicDiagram::EvalOneSample(vector<int> sp_indeces, bool sym_factor, bool permute) const {

    Num result = 1.;
    int nlines = lines.size();
    int nvertices = v_with_lines.size();

    // The value of a diagram is given by the product of its vertices,
    // multiplied by a symmetry factor, a global sign, and (possibly)
    // permutation operators.
    for (int iv=0; iv<nvertices; ++iv) {

        auto &vert = computable_vertices[iv];
        vector<int> bra, ket;
        
        for (auto ind : vert->GetOutLineIndeces() ) bra.push_back( sp_indeces[ind] );
        for (auto ind : vert->GetInLineIndeces() )  ket.push_back( sp_indeces[ind] );

        Num tmp = vert->Evaluate(bra, ket);
        if ( abs(tmp)<1e-5 ) return 0.;

        result *= tmp;
    }

    // Multiply by energy denominators
    if (this->type=="MBPT" && energy_denoms.size()>0 ) {

        double denoms = 1.;
        for (int id=0; id<energy_denoms.size(); ++id) {
            const auto &vert = energy_denoms[id];
            double sum_p = 0., sum_h = 0.;
            
            // By convention, holes outgoing and particles incoming
            for (auto ind_p : vert->GetInLineIndeces())   { sum_p += spbasis->GetSpEnergy( sp_indeces[ind_p] ); }
            for (auto ind_h : vert->GetOutLineIndeces() ) { sum_h += spbasis->GetSpEnergy( sp_indeces[ind_h] ); }
            denoms *= (sum_h - sum_p);
        }
        result /= denoms;
    }

    if (sym_factor) {
        result /= static_cast<double>(this->symmetry_factor);
    }
    result *= std::pow(-1, GetNumberOfHoleLines()-this->nloops );

    return result;

}

void DeterministicDiagram::Cleanup() {
    this->LabeledDiagram::Cleanup();
    spbasis = nullptr;
    computable_vertices.clear();
}

void DeterministicDiagramManager::SetSpBasis(shared_ptr<SpBasis> &spbasis_in) {
    spbasis = spbasis_in;
    for (auto& diag : diagrams) {
        DeterministicDiagram* det_diag = dynamic_cast<DeterministicDiagram*>(diag.get());
        if (det_diag) {
            det_diag->SetSpBasis(spbasis_in);
        }
    }
    return;
}

void DeterministicDiagramManager::SetSpBasis(unique_ptr<SpBasis> &spbasis_in) {
    std::shared_ptr<SpBasis> basePtr = std::move(spbasis_in);
    this->SetSpBasis(basePtr);
    return;
}

void DeterministicDiagramManager::Cleanup() {
    DiagramManager::Cleanup();
    spbasis = nullptr;
}

vector<Num> DeterministicDiagramManager::Compute() {
    vector<Num> results;
    for (auto& diag : diagrams) {
        DeterministicDiagram* det_diag = dynamic_cast<DeterministicDiagram*>(diag.get());
        if (det_diag) {
            Num res = det_diag->Compute();
            results.push_back(res);
        }
    }
    return results;
    // Process results as needed
}
