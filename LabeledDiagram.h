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
        VertexWithLine(int Nin, int Nout, bool virtual_flag=false, const std::string& name="", bool is_Hvertex=false, const std::string& latex_name="x");
        ~VertexWithLine() { Cleanup(); }

        void AddLineIndex(int index, bool out);

        vector<int> GetOutLineIndeces() const { return out_line_ind; }
        vector<int> GetInLineIndeces() const  { return in_line_ind; }
        vector<int> GetLineIndeces(const vector<unique_ptr<Line>> &lines, vector<bool> &in_out) const;

        virtual string GetTensorName(const vector<unique_ptr<Line>> &lines) const;

        void Print() const;
        virtual void Cleanup();

    protected:
        vector<int> out_line_ind, in_line_ind;
        vector<int> all_lines_indeces;

        string ListInIndeces(const vector<unique_ptr<Line>> &lines)  const;
        string ListOutIndeces(const vector<unique_ptr<Line>> &lines) const;
};


class LabeledDiagram: public Diagram {

    public:

        LabeledDiagram();
        LabeledDiagram(const IntMat& mat);
        LabeledDiagram(const IntMat& mat, const vector<Vertex>& vertices_in);
        LabeledDiagram(const IntMat& mat, const vector< unique_ptr<Vertex> >& vertices_in);

        virtual void Process() override;

        virtual bool CanGenerateLatexExpr() const { return true; }
        virtual string GetDiagramLatexExpression(bool show_ext=false) const;

        virtual void Cleanup() override;
        int GetNumberOfLines() const;
        int GetNumberOfHoleLines() const;
        void GetNumberOfPhLines(int &nh, int &np) const;
        void PrintLines() const;

    protected:
        vector< unique_ptr<Line> > lines;
        vector< unique_ptr<VertexWithLine> > v_with_lines;
        vector< unique_ptr<VertexWithLine> > energy_denoms;
        vector< vector<int> > equivalent_lines;

        int n_particle_lines, n_hole_lines;

        string GetInternalLinesString() const;
        virtual int CountLoops() const;

        virtual void ListLines();
        virtual void AssignLinesToVertices();
        virtual void FindLineType() = 0;
        virtual void AssignNamesToLines() = 0;
        virtual void FindDiagramExpression() = 0;

        vector< vector<int> > FindEquivalentLines();

    private:
};