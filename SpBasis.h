#pragma once

#include <memory>
#include <mpi.h>
#include <string>
#include <vector>

using std::vector;
using std::string;

typedef double Num;
const MPI_Datatype MPI_NUM_TYPE = MPI_DOUBLE;

class SpBasis {

    public:

        SpBasis();
        
        int GetNumberOfStates () const { return species.size(); }
        int GetNumberOfHoles() const;

        bool IsBuilt()  { return is_built; }
        bool IsHfDone() { return hf_done; }

        string GetName() const { return name; }
        void SetName(const string& name_in) { name = name_in; }
        
        virtual double GetRefEnergy() const;
        virtual double GetSpEnergy(int isp) const;

        virtual void PrintBasis() const;
        virtual void Build();

    protected:
        int nholes;
        int nstates;
        string name;

        bool is_built;
        bool hf_done;

        vector<double> kin_energies;
        vector<double> hf_energies;
        vector<int> species;
        vector<int> occupation;

        double GetEnergyDenom(int i, int a)  const;
        double GetEnergyDenom(int i, int j, int a, int b) const;
        double GetEnergyDenom(int i, int j, int k, int a, int b, int c) const;

        virtual Num ComputeF(int i1, int i2) const = 0;
        virtual bool CheckNonZero(int i1, int i2) const = 0;
        // virtual vector<int> FindCompatibleStates(int i1, string type="a") const;

        virtual Num ComputeV(int i1, int i2, int i3, int i4) const;
        virtual bool CheckNonZero(int i1, int i2, int i3, int i4) const;        
        virtual vector<int> FindCompatibleStates(int i1, int i2, int i3, string type="a") const;

        virtual void GetHfEnergies() = 0 ;

        virtual string PrintState(int index) const = 0;

        int CountHoles() const;
        virtual void Cleanup();

    private:

        friend class ComputableVertex;

};



