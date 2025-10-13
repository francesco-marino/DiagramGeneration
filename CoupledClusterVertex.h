#pragma once

#include "Vertex.h"


class T1Vertex : public Vertex {
    // Represents a T1 amplitude vertex in coupled cluster diagrams

    public:
        T1Vertex() : Vertex(1, 1, false, "T1", false) {}
        ~T1Vertex() {}

    protected:
        // Additional properties or methods specific to T1 vertices can be added here
};

class T2Vertex : public Vertex {
    // Represents a T2 amplitude vertex in coupled cluster diagrams

    public:
        T2Vertex() : Vertex(2, 2, false, "T2", false) {}
        ~T2Vertex() {}

    protected:
        // Additional properties or methods specific to T2 vertices can be added here
};
