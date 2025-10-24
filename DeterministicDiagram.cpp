
#include <iostream>
#include <map>

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

vector<int> ComputableVertex::GetLineIndecesWithConstraint(int constr_line, const vector<unique_ptr<Line>> &lines) {
    bool is_out_line = true;
    vector<int> all_lines;
    vector<bool> inout_lines;

    // First check if the constrained line is in/out.
    all_lines = this->GetLineIndeces(lines, inout_lines);
    for (int ii=0; ii<all_lines.size(); ++ii) {
        if (all_lines[ii]==constr_line) {
            is_out_line = inout_lines[ii];
            break;
        }
    }

    // We sort the lines so that:
    // if the constrined line is OUTgoing, we first list incoming lines, then outgoing ones;
    // if the constrined line is INcoming, outgoing lines are inserted first.
    // The constrained line comes last.
    vector<int> sorted_lines;
    if (is_out_line) {
        for (int ii : in_line_ind)  sorted_lines.push_back(ii);
        for (int oo : out_line_ind) sorted_lines.push_back(oo);
    }
    else {
        for (int oo : out_line_ind) sorted_lines.push_back(oo);
        for (int ii : in_line_ind)  sorted_lines.push_back(ii);
    }
    sorted_lines.push_back(constr_line);

    return sorted_lines;
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

bool ComputableVertex::CheckNonZero(vector<int> indeces) const {
    vector<int> bra, ket;
    for (const auto ll : GetOutLineIndeces() ) bra.push_back( indeces[ll] );
    for (const auto ll : GetInLineIndeces() )  ket.push_back( indeces[ll] );
    return CheckNonZero(bra, ket);
}

bool ComputableVertex::CheckNonZero(vector<int> bra, vector<int> ket) const {
    bool tmp = true;
    int nbra = this->GetNout();
    int nket = this->GetNin();
    if (bra.size()!=nbra || ket.size()!=nket) return false;

    if (nbra==2 && nket==2) {
        tmp = spbasis->CheckNonZero(bra[0], bra[1], ket[0], ket[1]);
    }
    else if (nbra==1 && nket==1) {
        tmp = spbasis->CheckNonZero(bra[0], ket[0]);
    }
    return tmp;
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

DeterministicDiagram::DeterministicDiagram() : LabeledDiagram() {
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

// TODO HERE This tourine is not working but i don't get why
void DeterministicDiagram::PrepareComputableVertices() {
    int nvertices = v_with_lines.size();
    for (int iv=0; iv<nvertices; ++iv) {
        unique_ptr<ComputableVertex> vert = make_unique<ComputableVertex>();
        vert->BuildFromVertex( v_with_lines[iv] );
        vert->SetSpBasis( spbasis );
        computable_vertices.push_back( std::move(vert) );
    }
    comp_vertices_ready = true;
    return;
}

void DeterministicDiagram::Process() {
    LabeledDiagram::Process();
    this->FindIndependentIndices();
}

Num DeterministicDiagram::Compute() {

    if (!IsBuilt()) return -1;
    //this->PrepareComputableVertices();

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

        bool non_zero = true;
        non_zero = non_zero && !AreThereEquivalentIndeces(slice);
        if (non_zero) { for (const auto& vert : computable_vertices) { if (!vert->CheckNonZero(slice)) { non_zero = false; break; } } }

        if ( non_zero ) {
            Num tmp = EvalOneSample(slice, true, false);
            result += tmp;
        }

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

//
// Find if there are (equivalent) lines with same index assigned. This implies, for antisymmetric tensors,
// that the result is zero.
//
bool DeterministicDiagram::AreThereEquivalentIndeces(const vector<int> indeces_in) const {
    bool identical_inds = false;
    if ( equivalent_lines.size()<=0 ) return false;

    for (const auto& group : this->equivalent_lines) {
        int l1, l2, l3;
        if ( group.size()<2 ) continue;
        l1 = group[0]; l2 = group[1];

        if (2==group.size()) {
            if (indeces_in[l1]==indeces_in[l2]) { identical_inds = true; break; }
        }
        else if ( 3==group.size() ) { 
            l3 = group[2]; 
            if (indeces_in[l1]==indeces_in[l2] || indeces_in[l1]==indeces_in[l3]) { identical_inds = true; break; }
        }
    }
    return identical_inds;
}


Num DeterministicDiagram::EvalOneSample(vector<int> sp_indeces, bool sym_factor, bool permute) const {

    Num result = 1.;
    int nlines = lines.size();
    int nvertices = v_with_lines.size();

    for (const auto& vert : computable_vertices) { if (!vert->CheckNonZero(sp_indeces)) return 0.; }

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
    result *= std::pow(-1, GetNumberOfHoleLines()-this->nloops );   // Sign

    return result;

}

void DeterministicDiagram::FindIndependentIndices() {

    int nlines = lines.size();
    int nvertices = v_with_lines.size();

    vector<bool> free_index(nlines, true);
    vector<int>  constr_index;
    vector<bool> constr_inout;

    // Iterate over all the vertices. For each vertex, one line must be constrained.
    // We simply constrain the first available.
    for (int iv=0; iv<nvertices-1; ++iv) {
        auto &vert = v_with_lines[iv];
        vector<bool> in_out;
        vector<int> lines_from_v = vert->GetLineIndeces(this->lines, in_out);
        int Nl = lines_from_v.size();
        int il = Nl-1;
        int line_now;

        while (il>=0) {
            line_now = lines_from_v[il];
            if ( free_index[line_now] ) { 
                free_index[line_now] = false; 
                break; 
            }
            --il;
        }
        // cout << "Vertex " << iv << "   Line " << line_now << "  " << lines[line_now]->GetLineName() << endl;
        constr_index.push_back(line_now);

    }
    constr_index.push_back(-1); // The last vertex is not constrained

    int nconstr = 0;
    for (bool bb : free_index) if (!bb) ++nconstr;
    if (nconstr!=nvertices-1) {
        cout << "Error! I have " << nconstr << " constrained indices " << endl;
    }

    for (int iv=0; iv<nvertices-1; ++iv) {
        int constr_ind = constr_index[iv];
        if (constr_ind<0) continue;
    
        auto &vert = v_with_lines[iv];
        vector<int> lines_from_v = vert->GetLineIndecesWithConstraint(constr_ind, this->vertices);
    }

}


void DeterministicDiagram::Cleanup() {
    this->LabeledDiagram::Cleanup();
    spbasis = nullptr;
    comp_vertices_ready = false;
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

    int ndiags = diagrams.size();
    if (ndiags==0) return vector<Num>();

    vector<Num> results(ndiags, 0.);
    
    // Parallelize
    for (int ii=rank; ii<ndiags; ii+=ntasks) {
        DeterministicDiagram* det_diag = dynamic_cast<DeterministicDiagram*>(diagrams[ii].get());
        if (det_diag) {
            Num res = det_diag->Compute();
            results[ii] = res;
        }
    }

    MPI_Allreduce(MPI_IN_PLACE, results.data(), ndiags, MPI_NUM_TYPE, MPI_SUM, MPI_COMM_WORLD);

    return results;
}
