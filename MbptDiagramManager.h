#pragma once

#include <memory>

#include "Diagram.h"
#include "DiagramManager.h"
#include "Vertex.h"

class MbptDiagramManager : public DiagramManager {
    // Manages the generation and storage of MBPT diagrams using adjacency matrices

    public:
        MbptDiagramManager() { DiagramManager::Cleanup(); };
        using DiagramManager::AddVertex;
        
    protected:

        std::unique_ptr<Diagram> CreateDiagram(const IntMat& mat) const override { return std::make_unique<Diagram>(mat); }

};