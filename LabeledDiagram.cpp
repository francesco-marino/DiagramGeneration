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

VertexWithLine::VertexWithLine() : Vertex() {}

VertexWithLine::VertexWithLine(int Nin, int Nout, bool virtual_flag, const std::string& name, bool is_Hvertex, const std::string& latex_name) : 
    Vertex(Nin, Nout, virtual_flag, name, is_Hvertex, latex_name) {}

void VertexWithLine::AddLine(Line& line, bool out) {
    if (out)   out_lines.push_back( make_unique<Line>(std::move(line)) );
    if (!out)  in_lines.push_back(  make_unique<Line>(std::move(line)) );
}

void VertexWithLine::AddLine(unique_ptr<Line>& line, bool out) {
    if (out)   out_lines.push_back( std::move(line) );
    if (!out)  in_lines.push_back(  std::move(line) );
}

void VertexWithLine::AddLine(unique_ptr<Line>& line, int index, bool out) {
    this->AddLine(line, out);
    this->AddLineIndex(index, out);
}

void VertexWithLine::AddLineIndex(int index, bool out) {
    if (out)    out_line_ind.push_back(index);
    if (!out)   in_line_ind.push_back(index);
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
    if ( out_indeces.size()>0) tmp += "^{" + out_indeces + "}";
    if ( in_indeces.size()>0)  tmp += "_{" + in_indeces  + "}";
    return tmp;
}

void VertexWithLine::Cleanup() {
    out_lines.clear();
    in_lines.clear();
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

void LabeledDiagram::AddLine(Line& line) {
    lines.push_back( make_unique<Line>( std::move(line) ) );
}

void LabeledDiagram::AddLine(unique_ptr<Line>& line) {
    lines.push_back( std::move(line) );
}


void LabeledDiagram::Process() {
    Diagram::Process();
    this->ListLines();
    this->AssignLinesToVertices();
    this->FindLineType();
    this->AssignNamesToLines();
    this->FindDiagramExpression();
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
        if ( !vert->IsAmplitude() ) continue;
        tmp += " " + vert->GetTensorName(lines);
    }
    tmp += " " + Hvert_name;

    if (show_ext) tmp = ext + tmp;
    //tmp = "$ " + tmp + " $";
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

    // for (auto &vwl: v_with_lines) vwl->Print();

    return;
}


int LabeledDiagram::GetNumberOfLines() const {
    return lines.size();
}

void LabeledDiagram::GetNumberOfPhLines(int &nh, int &np) const {
    nh = this->n_hole_lines;
    np = this->n_particle_lines;
    return;
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
    n_particle_lines = 0;
    n_hole_lines = 0;
}