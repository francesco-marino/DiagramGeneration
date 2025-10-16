#pragma once

#include "Vertex.h"


class T1Vertex : public Vertex {
    // Represents a T1 amplitude vertex in coupled cluster diagrams

    public:
        T1Vertex(bool is_virtual=false, const std::string& name="T1") : Vertex(1, 1, is_virtual, name, false) {
            std::string autoname = (is_virtual) ? "S" : "t";
            SetLatexSymbol(autoname);
        }
        ~T1Vertex() {}

    protected:
        // Additional properties or methods specific to T1 vertices can be added here
};;

class T2Vertex : public Vertex {
    // Represents a T2 amplitude vertex in coupled cluster diagrams

    public:
        T2Vertex(bool is_virtual=false, const std::string& name="T2") : Vertex(2, 2, is_virtual, name, false) {
            std::string autoname = (is_virtual) ? "D" : "t";
            SetLatexSymbol(autoname);
        }
        ~T2Vertex() {}

    protected:
        // Additional properties or methods specific to T2 vertices can be added here
};


class T3Vertex : public Vertex {
    // Represents a T2 amplitude vertex in coupled cluster diagrams

    public:
        T3Vertex(bool is_virtual=false, const std::string& name="T3") : Vertex(3, 3, is_virtual, name, false) {
            std::string autoname = (is_virtual) ? "T" : "t";
            SetLatexSymbol(autoname);
        }
        ~T3Vertex() {}

    protected:
        // Additional properties or methods specific to T2 vertices can be added here
};

