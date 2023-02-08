#include <cstdlib>

#include "mesh.hpp"

int main(int argc, char** argv){
    mesh::Mesh fromObj = mesh::Mesh::loadOBJ("media/objects/garg.obj");
    fromObj.toObj("media/objects/produced_garg.obj");
    exit(EXIT_SUCCESS);
}