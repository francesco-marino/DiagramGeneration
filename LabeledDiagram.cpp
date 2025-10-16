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

VertexWithLine::VertexWithLine(int Nin, int Nout, bool virtual_flag, const std::string& name, bool is_Hvertex) : 
    Vertex(Nin, Nout, virtual_flag, name, is_Hvertex) {}

void VertexWithLine::AddLine(Line& line, bool out) {
    if (out)   out_lines.push_back( make_unique<Line>(std::move(line)) );
    if (!out)  in_lines.push_back(  make_unique<Line>(std::move(line)) );
}

void VertexWithLine::AddLine(unique_ptr<Line>& line, bool out) {
    if (out)   out_lines.push_back( std::move(line) );
    if (!out)  in_lines.push_back(  std::move(line) );
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
            vertices[i]->GetName(), vertices[i]->IsHVertex() );
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