#pragma once

#include <memory>
#include <string>

class Vertex {

    public:
        Vertex() { Init(0, 0, false); }
        Vertex(int Nin, int Nout, bool virtual_flag=false, const std::string& name="", bool is_Hvertex=false) { Init(Nin, Nout, virtual_flag, is_Hvertex); SetName(name); }
        ~Vertex() {};

        int GetNin() const { return Nin; }
        int GetNout() const { return Nout; }
        bool IsVirtual() const { return isvirtual; }
        bool IsHVertex() const { return is_Hvertex; }

        void HamiltonianVertex() { is_Hvertex = true; }

        std::string GetLatexSymbol() const { return latex_symbol; }
        void SetLatexSymbol(const std::string& symbol) { latex_symbol = symbol; }
        
        std::string GetName() const { return name; }
        void SetName(const std::string& name) { this->name = name; }

        virtual bool operator==(const Vertex& other) const;
        bool operator==(const std::unique_ptr<Vertex>& other) const;

    protected:
        int Nin, Nout;
        bool isvirtual;
        std::string name;
        std::string latex_symbol;
        bool is_Hvertex;

        void Init(int Nin, int Nout, bool virtual_flag=false, bool is_Hvertex=false) {
            this->Nin = Nin;
            this->Nout = Nout;
            this->isvirtual = virtual_flag;
            this->is_Hvertex = is_Hvertex;
        }

};

class V1bVertex : public Vertex {
    public:
        V1bVertex() : Vertex(1, 1, false, "F", true) { };
};

class V2bVertex : public Vertex {
    public:
        V2bVertex() : Vertex(2, 2, false, "V", true) { };
};

class V3bVertex : public Vertex {
    public:
        V3bVertex() : Vertex(3, 3, false, "W", true) { };
};