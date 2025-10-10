#pragma once

#include <string>

class Vertex {

    public:
        Vertex() { Init(0, 0, false); }
        Vertex(int Nin, int Nout, bool virtual_flag=false, const std::string& name="") { Init(Nin, Nout, virtual_flag); SetName(name); }
        ~Vertex() {};

        int GetNin() const { return Nin; }
        int GetNout() const { return Nout; }
        bool IsVirtual() const { return isvirtual; }
        std::string GetLatexSymbol() const { return latex_symbol; }
        void SetLatexSymbol(const std::string& symbol) { latex_symbol = symbol; }
        std::string GetName() const { return name; }
        void SetName(const std::string& name) { this->name = name; }

    protected:
        int Nin, Nout;
        bool isvirtual;
        std::string name;
        std::string latex_symbol;

        void Init(int Nin, int Nout, bool virtual_flag=false) {
            this->Nin = Nin;
            this->Nout = Nout;
            this->isvirtual = virtual_flag;
        }

};

class V1bVertex : public Vertex {
    public:
        V1bVertex() : Vertex(1, 1, false) { };
};

class V2bVertex : public Vertex {
    public:
        V2bVertex() : Vertex(2, 2, false) { };
};

class V3bVertex : public Vertex {
    public:
        V3bVertex() : Vertex(3, 3, false) { };
};