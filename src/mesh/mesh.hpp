#pragma once

#include <vector>
#include <string>

#include "vertex.hpp"
#include "face.hpp"
#include "edge.hpp"

namespace mesh{

/**
 * The mesh class using winged mesh representation
*/
class Mesh{

    private:
        /**
         *  The number of vertices in the mesh
        */
        int mNbVertices;

        /**
         * The number of faces in the mesh
        */
        int mNbFaces;

        /**
         * The number of edges in the mesh
        */
        int mNbEdges;

        /**
         * The mesh's vertices
        */
        std::vector<mesh::Vertex> mVertices;

        /**
         * The mesh's faces
        */
        std::vector<mesh::Face> mFaces;

        /**
         * The mesh's edges
        */
        std::vector<mesh::Edge> mEdges;

    public:

        /**
         * The basic constructor
        */
        Mesh();

        /**
         * A constructor from an object file
         * @param file A file containing the mesh representation
        */
        Mesh(std::string filePath);

        /**
         * The destructor
        */
        ~Mesh();
};

}