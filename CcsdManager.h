#pragma once

#include "CoupledClusterDiagramManager.h"
#include "CoupledClusterDiagram.h"

class CcsdManager : public CoupledClusterDiagramManager {

    public :

        CcsdManager() { CoupledClusterDiagramManager::Cleanup(); }

        void Build(int trunc_level=2, bool include_3n=false, bool include_singles=true, int ntriples=1);
        void Build(bool do_ccd=true) override { Build(do_ccd, true); }
        void Build(bool do_ccd, bool only_connected);

    protected :

        void ConstructCcsdDiagrams(int trunc_level, bool include_3n=false, bool include_singles=true, int ntriples=1);
        void ConstructCcsdDiagrams(bool do_ccd=false);

    private : 
};