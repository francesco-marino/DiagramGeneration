#pragma once

#include <string>

class Vertex {

    public:
        Vertex() { Init(0, 0, false); }
        Vertex(int Nin, int Nout, bool virtual_flag=false) { Init(Nin, Nout, virtual_flag); }
        ~Vertex() {}

        int GetNin() const { return Nin; }
        int GetNout() const { return Nout; }
        bool IsVirtual() const { return isvirtual; }

    protected:
        int Nin, Nout;
        bool isvirtual;
        std::string latex_symbol;

        void Init(int Nin, int Nout, bool virtual_flag=false) {
            this->Nin = Nin;
            this->Nout = Nout;
            this->isvirtual = virtual_flag;
        }
        
};