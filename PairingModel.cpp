#include "PairingModel.h"

#include <iostream>

using std::to_string;
using std::cout;
using std::endl;


PairingModel::PairingModel() : SpBasis() {
    Cleanup();
}

bool PairingModel::CheckNonZero(int i1, int i2, int i3, int i4) const {
    bool tmp = SpBasis::CheckNonZero(i1, i2, i3, i4);
    if (!tmp) return false;
    if ( level[i1]!=level[i2] || level[i3]!=level[i4] ) return false;
    if ( spin[i1]+spin[i2] != spin[i3]+spin[i4] ) return false;
    return true;
}


Num PairingModel::ComputeV(int i1, int i2, int i3, int i4) const {
    if ( !CheckNonZero(i1,i2,i3,i4) ) return 0;
    Num pot = -0.5 * this->g;
    // Two cases: Spin(1)=Spin(3), Spin(2)=Spin(4),
    // or Spin(1)=Spin(4), Spin(2)=Spin(3).
    if ( spin[i1]==spin[i4] ) pot *= -1;
    return pot; 
}


void PairingModel::SetParams(int A, int D, double xi, double g) {
    this->A  = A;
    this->D  = D;
    this->xi = xi;
    this->g  = g;
    this->nstates = 2 * this->D;
    this->nholes  = this->A;
}

void PairingModel::AddState(int lev, int sp, int occ) {
    level.push_back( lev );
    spin.push_back( sp );
    species.push_back( sp );
    double kin = this->xi * (lev - 1);
    kin_energies.push_back( kin );
    if (0==occ || 1==occ) occupation.push_back( occ );
    return;
}

string PairingModel::PrintState(int index) const {
    if ( !is_built || index<0 || index>=nstates) return "";
    string tmp = 
        to_string(index) + "  " +
        to_string(occupation[index]) + "  " + 
        to_string(level[index]) + "  " + 
        to_string(spin[index]) + "  " + 
        to_string(kin_energies[index]);
    if (hf_done) {
        tmp += "  " + to_string(hf_energies[index]);
    }
    return tmp;
}


void PairingModel::Build() {
    
    int counter = 0;
    int occ_now = 0;

    for (int lev=1; lev<=D; ++lev) {
        for (int sp=-1; sp<=1; sp+=2) {
            occ_now = (counter<nholes) ? 1 : 0;
            this->AddState(lev, sp, occ_now);
            ++counter;
        }
    }

    GetHfEnergies();
    SpBasis::Build();
    return;
}


void PairingModel::GetHfEnergies() {

    hf_energies.clear();
    hf_energies = vector<double>(nstates);

    double ehf = 0.;
    for (int i=0; i<nstates; ++i) {
        ehf = kin_energies[i];
        if (1==occupation[i]) {
            ehf -= 0.5*g;
        }
        hf_energies[i] = ehf;
    }

    hf_done = true;
    return;
}


double PairingModel::GetMbpt2() const {
    
    double e2  = 0.;

    for (int i=0; i<nholes; ++i) {
        for (int j=i+1; j<nholes; ++j) {
            for (int a=nholes; a<nstates; ++a) {
                for (int b=a+1; b<nstates; ++b) {
                    if ( !CheckNonZero(i,j,a,b) ) continue;
                    double denom = GetEnergyDenom(i,j,a,b);
                    double mat = ComputeV(i,j,a,b);
                    e2 += mat*mat/denom;
                }
            }
        }
    }
    return e2;

}

double PairingModel::GetMbpt2Analytical() const {
    
    // Compare with:
    // https://github.com/hhurongzhe/pairFCIQMC/blob/main/data/data_pairing.txt

    double e2  = 0.;
    int fermi = level[nholes];

    for (int i=1; i<fermi; ++i) {
        for (int a=fermi; a<=D; ++a) {
            double denom = 2.*this->xi*(a - i) + this->g;
            e2 += 1./denom;
        }
    }
    e2 = -0.25 *g*g * e2;
    return e2;
}


double PairingModel::GetMbpt3() const {
    double e3 = 0.;
    double epp=0., ehh=0., eph=0.;

    // Iterate over amplitude t(ab,ij)
    for (int i=0; i<nholes; ++i) {
        for (int j=i+1; j<nholes; ++j) {

            for (int a=nholes; a<nstates; ++a) {
                for (int b=a+1; b<nstates; ++b) {

                    if ( !CheckNonZero(i,j,a,b) ) continue;
                    double denom = GetEnergyDenom(i,j,a,b);
                    double mat = ComputeV(i,j,a,b);
                    double t_abij = mat/denom;

                    // pp 
                    double X_ijab = 0.;
                    for (int c=nholes; c<nstates; ++c) {
                        for (int d=c+1; d<nstates; ++d) {
                            if ( !CheckNonZero(i,j,c,d) ) continue;
                            double t_ijcd = ComputeV(i,j,c,d)/GetEnergyDenom(i,j,c,d);
                            X_ijab += t_ijcd * ComputeV(c,d,a,b);
                        }
                    }
                    epp += X_ijab*t_abij;

                    // hh 
                    X_ijab = 0.;
                    for (int k=0; k<nholes; ++k) {
                        for (int l=k+1; l<nholes; ++l) {
                            if ( !CheckNonZero(i,j,k,l) ) continue;
                            double t_abkl = ComputeV(a,b,k,l)/GetEnergyDenom(k,l,a,b);
                            X_ijab += t_abkl * ComputeV(i,j,k,l);
                        }
                    }
                    ehh += X_ijab*t_abij;

                }
            }
        }
    }


    // Iterate over amplitude t(ab,ij)
    for (int i=0; i<nholes; ++i) {
        for (int j=0; j<nholes; ++j) {

            for (int a=nholes; a<nstates; ++a) {
                for (int b=nholes; b<nstates; ++b) {

                    if ( !CheckNonZero(i,j,a,b) ) continue;
                    double denom = GetEnergyDenom(i,j,a,b);
                    double mat = ComputeV(i,j,a,b);
                    double t_abij = mat/denom;
                    
                    // ph 
                    double X_ijab = 0.;
                    for (int k=0; k<nholes; ++k) {
                        for (int c=nholes; c<nstates; ++c) {
                            if ( !CheckNonZero(i,k,a,c) ) continue;
                            double t_ikac = ComputeV(i,k,a,c)/GetEnergyDenom(i,k,a,c);
                            X_ijab += t_ikac * ComputeV(c,j,k,b);
                        }
                    }
                    eph += X_ijab*t_abij;

                }
            }
        }
    }

    e3 = epp + ehh + eph;
    return e3;

}


void PairingModel::Cleanup() {
    SpBasis::Cleanup();
    A = D = 0;
    spin.clear();
    level.clear();
}