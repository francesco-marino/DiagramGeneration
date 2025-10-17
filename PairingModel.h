#pragma once

#include "SpBasis.h"


class PairingModel : public SpBasis {

    public:
        PairingModel();

        void SetParams(int A, int D, double xi, double g);
        virtual void Build() override;

        double GetMbpt2() const;
        double GetMbpt2Analytical() const;

        double GetMbpt3() const;

    protected:

        Num ComputeV(int i1, int i2, int i3, int i4) const override;
        bool CheckNonZero(int i1, int i2, int i3, int i4) const override;

        void AddState(int lev, int sp, int occ=-1);
        string PrintState(int index) const override;

        void GetHfEnergies() override;

        void Cleanup() override;

    private:
        double xi, g;
        int A, D;

        vector<int> spin;
        vector<int> level;

};
