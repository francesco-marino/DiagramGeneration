#include <iostream>

#include "Line.h"

using std::cout;
using std::endl;


Line::Line() { Cleanup(); }

Line::Line(const unique_ptr<Vertex>& Vin, const unique_ptr<Vertex>& Vout) {
    AssignVertices(Vin, Vout);
}

Line::Line(const Vertex& Vin, const Vertex& Vout) {
    AssignVertices(Vin, Vout);
}

void Line::AssignVertices(const unique_ptr<Vertex>& Vin, const unique_ptr<Vertex>& Vout) {
    this->Vin  = make_unique<Vertex>(*Vin);
    this->Vout = make_unique<Vertex>(*Vout);
    BuildLine();
}

void Line::AssignVertices(const Vertex& Vin, const Vertex& Vout) {
    this->Vin  = make_unique<Vertex>(Vin);
    this->Vout = make_unique<Vertex>(Vout);
    BuildLine();
}

void Line::BuildLine() {
    if ( Vin->IsVirtual() || Vout->IsVirtual()) is_ext_line = true;
    towards_ext = ( Vout->IsVirtual() ) ? true : false;
}

void Line::SetLineType(const string& lt) { 
    line_type = lt;
    if (line_type!="p" && line_type!="h" && line_type!="a") line_type = "a"; 
    is_type_set = true;
}


void Line::Print() const {
    cout << GetString() << endl;
}

string Line::GetString() const {
    string tmp = Vin->GetName() + " -> " + Vout->GetName();
    if (is_type_set) tmp += " (" +  line_type + ")";
    return tmp;
}
void Line::Cleanup() {
    Vin.reset();
    Vout.reset();
    is_ext_line = false;
    towards_ext = false;
    is_type_set = false;
}