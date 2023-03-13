#include <cstdlib>

#include "mesh.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    // mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/armadillo.obj");
    mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/garg.obj");
    // mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/chess_piece.obj");
    // mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/triangle.obj");
    // mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/cube.obj");
    // mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/venus.obj");
    // fromObj.checkCorrectness();
    // fromObj.toObj("media/objects/produced_chess_piece.obj");
    // std::printf("nbVertices:%d, nbFaces:%d\n", fromObj.mNbVertices, fromObj.mNbFaces);
    // fromObj.print();
    // fromObj.printStats();
    fromObj.triToQuad();
    // fromObj.printStats();
    // fromObj.print();
    fromObj.toObj("bin/objects/tmp.obj");
    exit(EXIT_SUCCESS);
}