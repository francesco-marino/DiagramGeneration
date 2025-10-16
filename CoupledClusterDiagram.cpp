#include <iostream>
#include <queue>

#include "CoupledClusterDiagram.h"
#include "Diagram.h"

using std::cout;

void CoupledClusterDiagram::CheckIsValid() {    

    // Iterate over vertices to check for virtual and H vertices
    int n_virtual = 0;
    int n_Hvertex = 0;

    for (int i=0; i<vertices.size(); ++i) {

        if (vertices[i]->IsVirtual()) {
            n_virtual++;
            pos_virtual_vertex = i;
        }
        if (vertices[i]->IsHVertex()) {
            n_Hvertex++;
            pos_Hvertex = i;
        }
    }

    has_virtual_vertex = (n_virtual > 0);
    has_Hvertex = (n_Hvertex ==1);

    // A valid Coupled Cluster diagram must have exactly one H vertex
    // and can have at most one virtual vertex
    is_valid = (has_Hvertex) && (n_virtual <= 1);
    if (!is_valid) return;

    // Amplitudes can not be connected to each other, but just to H or the virtual vertex.
    for (int i=0; i<vertices.size(); ++i) {
        if ( !vertices[i]->IsAmplitude() ) continue;
        for (int j=i+1; j<vertices.size(); ++j) {
            if ( !vertices[j]->IsAmplitude() ) continue;
            if ( adjacency_matrix(i,j) + adjacency_matrix(j,i) > 0 ) { 
                is_valid = false; 
                return; 
            }
        }
    }

    // H vertex must be connected to all other vertices (except the external one, if present)
    for (int j=0; j<vertices.size(); ++j) { 
        if ( !vertices[j]->IsAmplitude()) continue;
        if (adjacency_matrix(pos_Hvertex,j) + adjacency_matrix(j,pos_Hvertex)==0 ) {
            is_valid = false;
            return;
        }
    }

    // We look for equivalent vertices. We check the connectivity to the H vertex 
    // and impose an ordering to avoid redundant diagrams.
    for (int i=0; i<vertices.size(); ++i) {
        if ( !vertices[i]->IsAmplitude() ) continue;
        int conn_i_H = adjacency_matrix(i, pos_Hvertex) + adjacency_matrix(pos_Hvertex, i);
        if (conn_i_H == 0) { is_valid = false; return; }

        for (int j=i+1; j<vertices.size(); ++j) {
            if ( !vertices[j]->IsAmplitude() ) continue;
            if ( !vertices[i]->IsEquivalentTo(vertices[j]) ) continue; 
            
            int conn_j_H = adjacency_matrix(j, pos_Hvertex) + adjacency_matrix(pos_Hvertex, j);
            // First count the n. of connections with the H vertex. If equal, count n. of lines directed towards H.
            if ( conn_i_H < conn_j_H ) { is_valid = false; }
            else if ( conn_i_H == conn_j_H && adjacency_matrix(i, pos_Hvertex)<adjacency_matrix(j, pos_Hvertex) ) { is_valid = false; }
            
            if (!is_valid) return;
        }
    }
    
    return;
}

bool CoupledClusterDiagram::GetConnectivity(const IntMat &adj) {
    
    bool connected = Diagram::GetConnectivity(adj);
    if (!connected) return false;
    
    // Additional checks specific to Coupled Cluster diagrams can be added here
    
    if (!has_Hvertex) return false; // Must have H vertex
    return connected;
}


vector<int> CoupledClusterDiagram::FindSkeletonStructure(const IntMat& mat) const {
    // Implement skeleton structure extraction for Coupled Cluster diagrams
    // This is a placeholder implementation; actual logic will depend on the specific requirements for CC diagrams

    int n = mat.n_rows;
    vector<int> structure;

    // TODO HERE Implement

    return structure;
}

std::string CoupledClusterDiagram::GetVertexString() const {
    
    std::string vstr = "{";
    std::string virt = "0";
    for (const auto& v : vertices) {
        if (v->IsVirtual()) { virt = v->GetName(); continue; }
        vstr += v->GetName() + ", ";
    }
    vstr += "} -> " + virt;
    return vstr;
}


void CoupledClusterDiagram::FindLineType() {

    int n_lines = GetNumberOfLines();
    int n_vertices = GetNumberOfVertices();

    for (int vv=0; vv<n_vertices; ++vv) {
        auto &vert = this->v_with_lines[vv];
        vector<int> lout = vert->GetOutLineIndeces();
        vector<int> lin  = vert->GetInLineIndeces();

        if ( vert->IsAmplitude() || vert->IsVirtual() ) {
            for (auto &ll : lout) { lines[ll]->SetLineType("p"); }
            for (auto &ll : lin)  { lines[ll]->SetLineType("h"); }
        }
        
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


int CoupledClusterDiagram::GetExcitationLevel() const {

    int ext_level = 0;
    if (pos_virtual_vertex>=0) ext_level = vertices[pos_virtual_vertex]->GetNout();    // Excitation level

    return ext_level;
}


// TODO HERE    Think about moving back to LabeledDiagram
void CoupledClusterDiagram::AssignNamesToLines() { 

    int ext_level, np_lines, nh_lines, nvertices;
    std::queue<string> p_names, h_names;

    nvertices = GetNumberOfVertices();
    ext_level= GetExcitationLevel();
    GetNumberOfPhLines(nh_lines, np_lines);

    p_names.push("a"); p_names.push("b"); p_names.push("c"); p_names.push("d"); p_names.push("e"); p_names.push("f"); p_names.push("g"); p_names.push("h");
    h_names.push("i"); h_names.push("j"); h_names.push("k"); h_names.push("l"); h_names.push("m"); h_names.push("n"); h_names.push("o"); h_names.push("p");

    // Start from the external vertex.
    // Line that go OUT are HOLES. 
    // Line that go IN  are PARTICLES. 
    if (ext_level > 0) {
        auto &ext_vertex = v_with_lines[pos_virtual_vertex];

        for (auto ind_p : ext_vertex->GetInLineIndeces() ) {
            auto & ll_p = lines[ind_p];
            ll_p->SetLineName( p_names.front() );
            p_names.pop();
        }
        for (auto ind_h : ext_vertex->GetOutLineIndeces() ) {
            auto & ll_h = lines[ind_h];
            ll_h->SetLineName( h_names.front() );
            h_names.pop();
        }

    }

    // Now iterate over amplitude.
    // Now OUT lines are PARTICLES and IN lines are HOLES.
    for (int ii=0; ii<nvertices; ++ii) {
        auto &ampl = v_with_lines[ii];
        if ( !ampl->IsAmplitude() ) continue;

        for (auto ind_p : ampl->GetOutLineIndeces() ) {
            auto & ll_p = lines[ind_p];
            if ( ll_p->IsNameSet() ) continue;
            ll_p->SetLineName( p_names.front() );
            p_names.pop();
        }
        for (auto ind_h : ampl->GetInLineIndeces() ) {
            auto & ll_h = lines[ind_h];
            if ( ll_h->IsNameSet() ) continue;
            ll_h->SetLineName( h_names.front() );
            h_names.pop();
        }

    }

}


void CoupledClusterDiagram::FindDiagramExpression() {
    
}