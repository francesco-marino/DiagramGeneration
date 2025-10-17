#pragma once

#include <memory>
#include <string>
#include <vector>

using std::vector;
using std::string;

typedef double Num;

class SpBasis {

    public:

        SpBasis();
        
        int GetNumberOfStates () const { return species.size(); }
        int GetNumberOfHoles() const;

        bool IsBuilt()  { return is_built; }
        bool IsHfDone() { return hf_done; }

        virtual double GetRefEnergy() const;

        virtual void PrintBasis() const;
        virtual void Build();

    protected:
        int nholes;
        int nstates;

        bool is_built;
        bool hf_done;


        vector<double> kin_energies;
        vector<double> hf_energies;
        vector<int> species;
        vector<int> occupation;

        double GetEnergyDenom(int i, int j, int a, int b) const;

        virtual Num ComputeV(int i1, int i2, int i3, int i4) const;
        virtual bool CheckNonZero(int i1, int i2, int i3, int i4) const;

        virtual void GetHfEnergies() = 0 ;

        virtual string PrintState(int index) const = 0;

        int CountHoles() const;
        virtual void Cleanup();

    private:

};



