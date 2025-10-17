
#include <iostream>
#include "SpBasis.h"

using std::cout;
using std::endl;

SpBasis::SpBasis() {
    Cleanup();
}

int SpBasis::GetNumberOfHoles() const {
    return CountHoles();
}

int SpBasis::CountHoles() const {
    int count = 0;
    for (auto occ : occupation) { if(1==occ) ++count; }
    return count;
}

double SpBasis::GetEnergyDenom(int i, int j, int a, int b) const {
    if ( !CheckNonZero(i,j,a,b) ) return 0.;
    double res = hf_energies[i] + hf_energies[j] - hf_energies[a] - hf_energies[b];
    return res;   
}

double SpBasis::GetRefEnergy() const {
    
    if (!hf_done) return 0.;
    double ehf_tot = 0.0;
    double sum_eps=0., kin = 0.;

    for (int p=0; p<nholes; ++p) {
        kin += kin_energies[p];
        sum_eps += hf_energies[p];
    }
    ehf_tot = 0.5 * (kin +sum_eps);
    return ehf_tot;
}

bool SpBasis::CheckNonZero(int i1, int i2, int i3, int i4) const {
    if (!is_built) return false;
    if (i1<0 || i1>=nstates) return false;
    if (i2<0 || i2>=nstates) return false;
    if (i3<0 || i3>=nstates) return false;
    if (i4<0 || i4>=nstates) return false;
    if (i1==i2 || i3==i4)    return false;
    return true;
}

Num SpBasis::ComputeV(int i1, int i2, int i3, int i4) const {
    if ( !CheckNonZero(i1,i2,i3,i4) ) return 0.;
    return 0.;
}


void SpBasis::Build() { 
    nstates = GetNumberOfStates();
    nholes  = CountHoles();
    is_built = true; 
}

void SpBasis::PrintBasis() const {
    if (!is_built) return;
    cout << "N. states : " << GetNumberOfStates() << endl;
    cout << "N. holes  : " << GetNumberOfHoles() << endl;
    for (int i=0; i<nstates; ++i) {
        cout << PrintState(i) << endl;
    }
    return;
}

void SpBasis::Cleanup() {
    nstates = 0;
    nholes  = 0;
    is_built = false;
    hf_done  = false;
    kin_energies.clear();
    hf_energies.clear();
    species.clear();
    occupation.clear();
}