#include <iostream>

#include "Line.h"

using std::cout;
using std::endl;


Line::Line() { Cleanup(); }

Line::Line(const unique_ptr<Vertex>& Vin, const unique_ptr<Vertex>& Vout) {
    Cleanup();
    AssignVertices(Vin, Vout);
}

Line::Line(const Vertex& Vin, const Vertex& Vout) {
    Cleanup();
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


void Line::SetLineName(const string& ln) {
    line_name = ln;
    is_name_set = true;
}

int Line::GetSpIndex() const {
    int tmp = (sp_index_set) ? sp_index : -1;
    return tmp;
}

void Line::SetDependencies(bool free_in, const vector<int>& deps) {
    is_free = free_in;
    for (const int dd : deps) dependencies.push_back(dd);
}


void Line::Print() const {
    cout << GetString() << endl;
}

string Line::GetString() const {
    string tmp = "";
    if (is_name_set) {
        string tmp_name = line_name;
        if ( !this->IsExternalLine() ) tmp_name = "(" + tmp_name + ")";
        tmp += tmp_name + " : ";
    }
    tmp += Vin->GetName() + " -> " + Vout->GetName();
    if (is_type_set) tmp += " (" +  line_type + ")";
    return tmp;
}

void Line::Cleanup() {
    Vin.reset();
    Vout.reset();
    is_ext_line = false;
    towards_ext = false;
    is_type_set = false;
    is_name_set = false;
    Vin_index   = -1;
    Vout_index  = -1;
    sp_index_set = false;
    sp_index = -1;
    dependencies.clear();
    is_free = false;
}