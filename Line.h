#pragma once

#include <memory>
#include <string>

#include "Vertex.h"

using std::unique_ptr;
using std::make_unique;
using std::string;

class Line {
    public:

        Line();
        Line(const Vertex& Vin, const Vertex& Vout);
        Line(const unique_ptr<Vertex>& Vin, const unique_ptr<Vertex>& Vout);

        bool IsExternalLine() const { return is_ext_line; }
        bool PointsToExtern() const { return towards_ext; }
        bool IsTypeSet() const { return is_type_set; }
        virtual void Print() const;

        void SetLineType(const string& lt);
        string GetLineType() const { return line_type; }

    protected:
    
        unique_ptr<Vertex> Vin, Vout;
        bool is_ext_line;
        bool towards_ext;   // true if it is an external line and points upward
        string latex_name;
        string line_type;     // p, h, a
        bool is_type_set;

        void AssignVertices(const Vertex& Vin, const Vertex& Vout);
        void AssignVertices(const unique_ptr<Vertex>& Vin, const unique_ptr<Vertex>& Vout);
        void BuildLine();

        string GetString() const;

    private:
        void Cleanup();

};