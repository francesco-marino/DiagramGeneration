#pragma once

#include "CoupledClusterDiagram.h"
#include "Diagram.h"
#include "DiagramManager.h"


class CoupledClusterDiagramManager : public DiagramManager {
    // Manages the generation and storage of coupled cluster diagrams using adjacency matrices

    public:
        CoupledClusterDiagramManager() { DiagramManager::Cleanup(); }

    protected:

        std::unique_ptr<Diagram> CreateDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in) const override { return std::make_unique<CoupledClusterDiagram>(mat, vertices_in); }
        std::unique_ptr<Diagram> CreateDiagram(const IntMat& mat, const vector<Vertex>& vertices_in) const override { return std::make_unique<CoupledClusterDiagram>(mat, vertices_in); }

        void PostProcessDiagrams() override {};
};