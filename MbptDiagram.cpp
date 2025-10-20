#include "MbptDiagram.h"

#include <iostream>
#include <queue>

using std::cout;
using std::endl;


void MbptDiagram::Process() {
    LabeledDiagram::Process();
    this->FindEnergyDenoms();
}

vector<int> MbptDiagram::FindSkeletonStructure(const IntMat& mat) const {
    // Follow Shavitt&Bartlett, Sec. 5.7.2
    int row = 0;
    int n = mat.n_rows;
    vector<int> skeleton;

    for (int col=0; col<n; ++col) {
        if (col == row) continue;
        int conn = mat(row, col) + mat(col, row);
        skeleton.push_back(conn);
    }

    return skeleton;
}

void MbptDiagram::FindLineType() {

    int n_lines = GetNumberOfLines();
    int n_vertices = GetNumberOfVertices();

    for (auto& ll: lines) {
        int vin  = ll->getInIndex();
        int vout = ll->getOutIndex();
        // By convention, a line going to a vertex with higher 
        // identifier is a PARTICLE.
        if ( vout>vin ) ll->SetLineType("p");
        if ( vout<vin ) ll->SetLineType("h");
        
    }

    for (const auto& ll: lines) {
        if (ll->GetLineType()=="p") ++n_particle_lines;
        if (ll->GetLineType()=="h") ++n_hole_lines;
    }


    // DEBUG
    for (auto &ll: lines) {
        if (!ll->IsTypeSet()) {
            cout << "TYPE NOT SET" << std::endl;
        }
    }

}


void MbptDiagram::AssignNamesToLines() { 

    int nvertices;
    std::queue<string> p_names, h_names;

    nvertices = GetNumberOfVertices();

    p_names.push("a"); p_names.push("b"); p_names.push("c"); p_names.push("d"); p_names.push("e"); p_names.push("f"); p_names.push("g"); p_names.push("h");
    h_names.push("i"); h_names.push("j"); h_names.push("k"); h_names.push("l"); h_names.push("m"); h_names.push("n"); h_names.push("o"); h_names.push("p");

    for (int ii=0; ii<nvertices; ++ii) {
        auto &vert = v_with_lines[ii];

        // Assign names to the outgoing lines from every vertex.
        for (auto ind_out : vert->GetOutLineIndeces() ) {
            auto & ll_out = lines[ind_out];
            if ( ll_out->IsNameSet() ) continue;
            if ( ll_out->GetLineType()=="p" ) {
                ll_out->SetLineName( p_names.front() );
                p_names.pop();
            }
            else if ( ll_out->GetLineType()=="h" ) {
                ll_out->SetLineName( h_names.front() );
                h_names.pop();
            }
        }
    }

    return;
}

void MbptDiagram::FindEnergyDenoms() {

    int nvertices = GetNumberOfVertices();
    int nlines = GetNumberOfLines();

    vector<int> virt_line_p, virt_line_h;

    // The algorithm works as follows. We draw imaginary horizontal lines
    // between vertex i and i+1, and search for lines that go though this horizontal
    // cut. 
    // Conventionally, the denominator will be a tensor \epsilon^{ijk...}_{abc...}
    for (int iv=0; iv<nvertices-1; ++iv) {
        unique_ptr<VertexWithLine> denom = make_unique<VertexWithLine>();
        denom->SetLatexSymbol("\\epsilon");
        energy_denoms.push_back( std::move(denom) );
    }

    for (int il=0; il<nlines; ++il) {
        auto& line = lines[il];
        int init, fin;

        init = line->getInIndex();
        fin  = line->getOutIndex();

        // Example: particle line with init=1, fin=4. Then the line will contribute to 
        // denoms 1, 2 and 3.
        if ("p"==line->GetLineType()) {
            for (int iv=init; iv<fin; ++iv) {
                energy_denoms[iv]->AddLineIndex(il, false);
            }
        }
        // Example: hole lines with init=3, fin=1. It will then contribute to denoms 1,2
        else if ("h"==line->GetLineType()) {
            for (int iv=fin; iv<init; ++iv) {
                energy_denoms[iv]->AddLineIndex(il, true);
            }
        }
    }
    
    return;
}


// TODO HERE
void MbptDiagram::FindDiagramExpression() {

}


// TODO HERE This still needs work
bool MbptDiagram::IsConjugateToDiag(const MbptDiagram& other) { 
    bool check_adjacency = arma::all(arma::vectorise(adjacency_matrix.t() == other.adjacency_matrix));
    bool check_vertices = true;

    has_conjugate = has_conjugate || check_adjacency;
    return has_conjugate;
}


void MbptDiagram::CheckIsValid() {    
    is_valid = true; // Assume all MBPT diagrams are valid in this context and run no additional checks
    return;
}

void MbptDiagram::Cleanup() {
    LabeledDiagram::Cleanup();
    energy_denoms.clear();
}


void MbptDiagram::Print() const {
    Diagram::Print();
    if (IsSelfConjugate()) {
        std::cout << "Self-conjugate.\n";
    } else if (HasConjugate()) {
        std::cout << "Has pair-conjugate.\n";
    }
    return;
}

string MbptDiagram::GetDiagramLatexExpression(bool show_ext) const {
    string tmp = LabeledDiagram::GetDiagramLatexExpression(show_ext);
    for (const auto& dd : energy_denoms) {
        tmp += " " + dd->GetTensorName(lines);
    }
    return tmp;
}
        