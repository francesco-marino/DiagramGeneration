#pragma once

#include <memory>
#include <vector>

#include "Diagram.h"
#include "Line.h"
#include "Vertex.h"

using std::unique_ptr;
using std::make_unique;

// TODO HERE Change name
class VertexWithLine : public Vertex {
    public:

        VertexWithLine();
        VertexWithLine(int Nin, int Nout, bool virtual_flag=false, const std::string& name="", bool is_Hvertex=false);
        ~VertexWithLine() { Cleanup(); }

        void AddLine(Line& line, bool out);
        void AddLine(unique_ptr<Line>& line, bool out);
        void AddLineIndex(int index, bool out);

        vector<int> GetOutLineIndeces() const { return out_line_ind; }
        vector<int> GetInLineIndeces() const {  return in_line_ind; }

        void Print() const;

    protected:
        vector< unique_ptr<Line> > out_lines, in_lines;
        vector<int> out_line_ind, in_line_ind;

        void Cleanup();
};


class LabeledDiagram: public Diagram {

    public:

        LabeledDiagram();
        LabeledDiagram(const IntMat& mat);
        LabeledDiagram(const IntMat& mat, const vector<Vertex>& vertices_in);
        LabeledDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in);

        void AddLine(unique_ptr<Line>& line);
        void AddLine(Line& line);

        virtual void Process();

        virtual void Cleanup();
        int GetNumberOfLines() const;
        void GetNumberOfPhLines(int &nh, int &np) const;
        void PrintLines() const;

    protected:
        vector< unique_ptr<Line> > lines;
        vector< unique_ptr<VertexWithLine> > v_with_lines;

        int n_particle_lines, n_hole_lines;

        virtual void ListLines();
        virtual void AssignLinesToVertices();
        virtual void FindLineType() = 0;
        virtual void AssignNamesToLines() = 0;
        // virtual void FindDiagramExpression() = 0;

    private:
};