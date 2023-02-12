#include <cstdlib>

#include "mesh.hpp"

int main(int argc, char** argv){
    mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/cube.obj");
    // fromObj.toObj("media/objects/produced_chess_piece.obj");
    // std::printf("nbVertices:%d, nbFaces:%d\n", fromObj.mNbVertices, fromObj.mNbFaces);
    // fromObj.print();
    fromObj.triToQuad();
    // fromObj.print();
    // fromObj.toObj("media/objects/produced_cube.obj");
    exit(EXIT_SUCCESS);
}