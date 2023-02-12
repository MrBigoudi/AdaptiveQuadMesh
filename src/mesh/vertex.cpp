#include "vertex.hpp"

int mesh::Vertex::ID_CPT = 0;

std::string mesh::Vertex::toString(){
    char buffer[50];
    sprintf(buffer, "v%d -> e%d, toDel = %d", mId, mEdge->mId, mToDelete);
    return buffer;
}

void mesh::Vertex::print(){
    fprintf(stdout, "%s\n", toString().c_str());
}