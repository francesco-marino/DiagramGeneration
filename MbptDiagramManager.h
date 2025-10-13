#pragma once

#include <memory>

#include "Diagram.h"
#include "DiagramManager.h"
#include "MbptDiagram.h"
#include "Vertex.h"

class MbptDiagramManager : public DiagramManager {
    // Manages the generation and storage of MBPT diagrams using adjacency matrices

    public:
        MbptDiagramManager() { DiagramManager::Cleanup(); };
        using DiagramManager::AddVertex;
        
    protected:

        std::unique_ptr<Diagram> CreateDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) const override { return std::make_unique<MbptDiagram>(mat, vertices_in); }
        std::unique_ptr<Diagram> CreateDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) const override { return std::make_unique<MbptDiagram>(mat, vertices_in); }

        void PostProcessDiagrams() override; 
};