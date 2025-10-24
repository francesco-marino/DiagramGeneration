#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Vertex.h"

using std::unique_ptr;
using std::make_unique;
using std::string;
using std::vector;

class Line {
    public:

        Line();
        Line(const Vertex& Vin, const Vertex& Vout);
        Line(const unique_ptr<Vertex>& Vin, const unique_ptr<Vertex>& Vout);

        bool IsExternalLine() const { return is_ext_line; }
        bool PointsToExtern() const { return towards_ext; }
        bool IsNameSet() const { return is_name_set; }
        bool IsTypeSet() const { return is_type_set; }
        virtual void Print() const;

        void SetLineIndeces(int i_in, int i_out) { Vin_index = i_in; Vout_index = i_out;}
        int getInIndex()  const { return Vin_index; }
        int getOutIndex() const { return Vout_index; }

        unique_ptr<Vertex>& GetInVertex()  { return Vin; }
        unique_ptr<Vertex>& GetOutVertex() { return Vout; }

        void SetLineType(const string& lt);
        string GetLineType() const { return line_type; }

        void SetLineName(const string& ln);
        string GetLineName() const { return line_name; }

        void SetSpIndex(int isp) { sp_index=isp; sp_index_set=true; }
        int GetSpIndex() const;

        bool IsFreeIndex() const { return is_free; }
        void SetDependencies(bool free_in, const vector<int>& deps);

    protected:
    
        unique_ptr<Vertex> Vin, Vout;
        int Vin_index, Vout_index;
        bool is_ext_line;
        bool towards_ext;   // true if it is an external line and points upward
        string latex_name;
        string line_type;     // p, h, a
        string line_name;
        bool is_type_set;
        bool is_name_set;

        int sp_index;
        bool sp_index_set;
        bool is_free;   // Free index or constrained to other indeces
        vector<int> dependencies;

        void AssignVertices(const Vertex& Vin, const Vertex& Vout);
        void AssignVertices(const unique_ptr<Vertex>& Vin, const unique_ptr<Vertex>& Vout);
        void BuildLine();

        string GetString() const;

    private:
        void Cleanup();

};