#include "Vertex.h"

bool Vertex::operator==(const Vertex& other) const {
    bool res = (this->Nin == other.Nin) && (this->Nout == other.Nout);
    return res;
}

bool Vertex::operator==(const std::unique_ptr<Vertex>& other) const {
    return (*this == *other);
}