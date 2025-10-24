#include <algorithm>
#include <iostream>
#include <string>
#include <queue>

#include "LabeledDiagram.h"
#include "Line.h"
#include "Vertex.h"

using std::cout;
using std::endl;
using std::string;

VertexWithLine::VertexWithLine() : Vertex() { this->Cleanup(); }

VertexWithLine::VertexWithLine(int Nin, int Nout, bool virtual_flag, const std::string& name, bool is_Hvertex, const std::string& latex_name) : 
    Vertex(Nin, Nout, virtual_flag, name, is_Hvertex, latex_name) {
    this->Cleanup();
    }

void VertexWithLine::AddLineIndex(int index, bool out) {
    if (out)    out_line_ind.push_back(index);
    if (!out)   in_line_ind.push_back(index);
    Nin  = in_line_ind.size();
    Nout = out_line_ind.size();
}

void VertexWithLine::Print() const {
    cout << this->GetName();
    cout << "\nOutgoing lines : ";
    for (int i=0; i<out_line_ind.size(); ++i) cout << out_line_ind[i] << " ";
    cout << "\nIncoming lines : ";
    for (int i=0; i<in_line_ind.size(); ++i)  cout << in_line_ind[i] << " ";
    cout << std::endl;
    
}

string VertexWithLine::ListOutIndeces(const vector<unique_ptr<Line>> &lines) const {
    string tmp = "";
    for (const auto& i_out: out_line_ind) {
        const auto& ll = lines[i_out];
        if (ll->IsNameSet()) tmp += ll->GetLineName();
    }
    return tmp;
}

string VertexWithLine::ListInIndeces(const vector<unique_ptr<Line>> &lines) const {
    string tmp = "";
    for (const auto& i_in: in_line_ind) {
        const auto& ll = lines[i_in];
        if (ll->IsNameSet()) tmp += ll->GetLineName();
    }
    return tmp;
}

//
// Get the name of the object with proper in/out indeces.
//
string VertexWithLine::GetTensorName(const vector<unique_ptr<Line>> &lines) const {
    string out_indeces = ListOutIndeces(lines);
    string in_indeces  = ListInIndeces(lines);
    string tmp = GetLatexSymbol();
    if ( this->IsVirtual() ) { std::swap(out_indeces, in_indeces); }
    if ( out_indeces.size()>0) tmp += "^{" + out_indeces + "}";
    if ( in_indeces.size()>0)  tmp += "_{" + in_indeces  + "}";
    return tmp;
}

void VertexWithLine::Cleanup() {
    out_line_ind.clear();
    in_line_ind.clear();
}


LabeledDiagram::LabeledDiagram() {
    Diagram();
}

LabeledDiagram::LabeledDiagram(const IntMat& mat) : Diagram(mat) { 
}

LabeledDiagram::LabeledDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) : Diagram(mat, vertices_in) { 
}

LabeledDiagram::LabeledDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) : Diagram(mat, vertices_in) { 
}


void LabeledDiagram::Process() {
    Diagram::Process();
    this->ListLines();
    this->AssignLinesToVertices();
    this->FindLineType();
    this->AssignNamesToLines();
    this->nloops = this->CountLoops();
    this->symmetry_factor = this->FindSymmetryFactor();
    this->equivalent_lines = this->FindEquivalentLines();
}

string LabeledDiagram::GetInternalLinesString() const {
    vector<string> internal_indeces;
    for (const auto& ll : lines) {
        if (ll->IsExternalLine()) continue;
        internal_indeces.push_back(ll->GetLineName());
    }
    std::sort(internal_indeces.begin(), internal_indeces.end());

    // Merge into a single string (with a separator, e.g., comma or space)
    std::string merged;
    for (const auto& w : internal_indeces) {
        //if (!merged.empty()) merged += ", ";  // separator
        merged += w;
    }
    return merged;
}

string LabeledDiagram::GetDiagramLatexExpression(bool show_ext) const {
    
    string tmp = "";
    int sign = ( nloops + this->GetNumberOfHoleLines() )%2;
    
    string sym = "";
    if (this->symmetry_factor>1) { sym = "\\frac{1}{" + std::to_string(symmetry_factor) + "} "; }
    
    string Hvert_name = (pos_Hvertex>=0) ? v_with_lines[pos_Hvertex]->GetTensorName(lines) : "";
    string int_lines = GetInternalLinesString();

    string tmp_sum = "";
    if (int_lines.length()>0) tmp_sum = "\\sum_{" + int_lines + "}" ;

    string ext = "";
    if ( HasVirtualVertex() ) {
        ext = v_with_lines[pos_virtual_vertex]->GetTensorName(lines) + " = ";
    }
    else {
        ext = "E = ";   // Conventionally call "E"
    }

    tmp =  tmp_sum;
    for (const auto& vert : v_with_lines) {
        if (GetType()=="CC" && !vert->IsAmplitude() ) continue;
        // TODO HERE Add mbpt energy denominators
        tmp += " " + vert->GetTensorName(lines);
    }
    tmp += " " + Hvert_name;
    tmp = sym + tmp;
    if (sign==1)  tmp = "- " + tmp;
    if (show_ext) tmp = ext + tmp;

    return tmp;

}


//
// Find all lines in the diagram
// 
void LabeledDiagram::ListLines() {

    int n = adjacency_matrix.n_cols;

    for (int i=0; i<n; ++i) {
        for (int j=0; j<n; ++j) {
            int mult = adjacency_matrix.at(i,j);
            for (int k=0; k<mult; ++k) {
                unique_ptr<Line> line = make_unique<Line>(vertices[i], vertices[j]);
                line->SetLineIndeces(i, j);
                lines.push_back( std::move(line) );
            }
        }
    }

    return;

}

void LabeledDiagram::AssignLinesToVertices() {
    
    int n_vertices = vertices.size();
    int n_lines = lines.size();

    v_with_lines.clear();

    for (int i=0; i<n_vertices; ++i) {
        unique_ptr<VertexWithLine> vwl = make_unique<VertexWithLine>(
            vertices[i]->GetNin(), vertices[i]->GetNout(), vertices[i]->IsVirtual(), 
            vertices[i]->GetName(), vertices[i]->IsHVertex(), vertices[i]->GetLatexSymbol() );
        v_with_lines.push_back( std::move(vwl) );
    }

    for (int ll=0; ll<n_lines; ++ll) {
        
        // Line goes from v(in) to v(out).
        auto &line = lines[ll];
        int vin  = line->getInIndex();
        int vout = line->getOutIndex();
        
        // So vertex v(out) sees line as incoming
        v_with_lines[vout]->AddLineIndex(ll, false);
        v_with_lines[vin]->AddLineIndex( ll, true);
    }

    return;
}

// TODO HERE Hard-coded algorithm, not nice...
int LabeledDiagram::CountLoops() const {
    int nloops = 0;
    int nvertices = this->GetNumberOfVertices();
    int nlines = this->GetNumberOfLines();

    vector<bool> visited(nvertices), lines_visited(nlines);

    for (int i=0; i<nvertices; ++i) visited[i] = false;
    for (int i=0; i<nlines; ++i)    lines_visited[i] = false;

    // First, we check pairs of vertices connected to each other.    
    for (int il1=0; il1<nlines; ++il1) {
        if ( lines_visited[il1] ) continue;
        int vin1  = lines[il1]->getInIndex();
        int vout1 = lines[il1]->getOutIndex();

        for (int il2=0; il2<nlines; ++il2) {
            if ( lines_visited[il1] ) continue;
            if ( lines_visited[il2] ) continue;

            int vin2  = lines[il2]->getInIndex();
            int vout2 = lines[il2]->getOutIndex();
            if (vin2 != vout1) continue;

            if (vout2==vin1) { 
                lines_visited[il1] = true;
                lines_visited[il2] = true;
                ++nloops;
                break;
            }

            for (int il3=0; il3<nlines; ++il3) {
                if ( lines_visited[il1] ) continue;
                if ( lines_visited[il2] ) continue;
                if ( lines_visited[il3] ) continue;

                int vin3  = lines[il3]->getInIndex();
                int vout3 = lines[il3]->getOutIndex();
                if (vin3 != vout2) continue;

                if (vout3==vin1) {
                    lines_visited[il1] = true;
                    lines_visited[il2] = true;
                    lines_visited[il3] = true;
                    ++nloops;
                    break;
                } 

                for (int il4=0; il4<nlines; ++il4) {
                    if ( lines_visited[il1] ) continue;
                    if ( lines_visited[il2] ) continue;
                    if ( lines_visited[il3] ) continue;
                    if ( lines_visited[il4] ) continue;

                    int vin4  = lines[il4]->getInIndex();
                    int vout4 = lines[il4]->getOutIndex();
                    if (vin4 != vout3) continue;

                    if (vout4==vin1) {
                        lines_visited[il1] = true;
                        lines_visited[il2] = true;
                        lines_visited[il3] = true;
                        lines_visited[il4] = true;

                        ++nloops;
                        break;
                    } 

                    for (int il5=0; il5<nlines; ++il5) {
                        if ( lines_visited[il1] ) continue;
                        if ( lines_visited[il2] ) continue;
                        if ( lines_visited[il3] ) continue;
                        if ( lines_visited[il4] ) continue;
                        if ( lines_visited[il5] ) continue;

                        int vin5  = lines[il5]->getInIndex();
                        int vout5 = lines[il5]->getOutIndex();
                        if (vin5 != vout4) continue;

                        if (vout5==vin1) {
                            lines_visited[il1] = true;
                            lines_visited[il2] = true;
                            lines_visited[il3] = true;
                            lines_visited[il4] = true;
                            lines_visited[il5] = true;

                            ++nloops;
                            break;
                        }
                    }
                }
            }

        } 
    }


    for (int il=0; il<nlines; ++il) {
        if (!lines_visited[il]) {
            cout << "Line " << il << " ( " << lines[il]->GetLineName() << ") was not visited" << endl;
        }
    }
    
    return nloops;
}


int LabeledDiagram::GetNumberOfLines() const {
    return lines.size();
}

int LabeledDiagram::GetNumberOfHoleLines() const {
    int count = 0;
    for (const auto& ll: lines) {
        if (ll->GetLineType()=="h") ++count;
    }
    return count;
}

void LabeledDiagram::GetNumberOfPhLines(int &nh, int &np) const {
    nh = this->n_hole_lines;
    np = this->n_particle_lines;
    return;
}

vector< vector<int> > LabeledDiagram::FindEquivalentLines() {
    
    vector< vector<int> > equivs;
    int nlines = lines.size();

    for (int i1=0; i1<nlines; ++i1) {
        for (int i2=i1+1; i2<nlines; ++i2) {
            if ( lines[i1]->getOutIndex() != lines[i2]->getOutIndex()) continue;
            if ( lines[i1]->getInIndex()  != lines[i2]->getInIndex())  continue;

            // TODO HERE Extend to 3n interactions
            equivs.push_back( vector<int>({i1, i2}) );

            for (int i3=i2+1; i3<nlines; ++i3) {
                if ( lines[i1]->getOutIndex() != lines[i3]->getOutIndex()) continue;
                if ( lines[i1]->getInIndex()  != lines[i3]->getInIndex())  continue;
                equivs.push_back( vector<int>({i1, i2, i3}) );
            }
        } 
    }
    return equivs;

}


void LabeledDiagram::PrintLines() const {
    int n_lines = GetNumberOfLines();
    cout << "There are " << n_lines << " lines in the diagram" << endl;
    for (auto & ll : lines) ll->Print();
    cout << endl;
}

void LabeledDiagram::Cleanup() {

    Diagram::Cleanup();
    lines.clear();
    v_with_lines.clear();
    energy_denoms.clear();
    equivalent_lines.clear();
    n_particle_lines = 0;
    n_hole_lines = 0;
}