#pragma once

#include <vector>
#include <string>

#include "vertex.hpp"
#include "face.hpp"
#include "edge.hpp"
#include "vector3.hpp"

namespace mesh{

/**
 * The mesh class using winged mesh representation
*/
class Mesh{

    public:
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
        std::vector<mesh::Vertex*> mVertices;

        /**
         * The mesh's faces
        */
        std::vector<mesh::Face*> mFaces;

        /**
         * The mesh's edges
        */
        std::vector<mesh::Edge*> mEdges;

    public:

        /**
         * The basic constructor
         * @param nbVertices The number of vertices in the mesh
         * @param nbFaces The number of faces in the mesh
         * @param nbEdges The number of edges in the mesh
         * @param vertices The mesh's vertices
         * @param faces The mesh's faces
         * @param edges The mesh's edges
        */
        Mesh(int nbVertices, int nbFaces, int nbEdges, 
                std::vector<mesh::Vertex*> vertices, 
                std::vector<mesh::Face*> faces, 
                std::vector<mesh::Edge*> edges){
            mNbVertices = nbVertices;
            mNbFaces = nbFaces;
            mNbEdges = nbEdges;
            mVertices = vertices;
            mFaces = faces;
            mEdges = edges;
        };

        /**
         * The destructor
        */
        ~Mesh(){};

        /**
         * Creat a mesh from an object file
         * @param file A file containing the mesh representation
         * @exception Invalid_Argument if the file is not correct
         * @return A new mesh
        */
        static Mesh loadOBJ(std::string file);

        /**
         * Create a mesh from vertices and faces of an obj file
         * @param vertices The vertices from the obj file
         * @param faces The faces from the obj file
         * @return A new mesh
        */
        static mesh::Mesh objToMesh(std::vector<maths::Vector3*> &vertices, std::vector<std::vector<int>> &faces);

        /**
         * Create an obj file from a mesh
         * @param file The produced file
         * @exception Invalid_Argument if the file is not correct
        */
        void toObj(std::string file);

        /**
         * Extract the vertices from the mesh's faces
         * @return A matrix of vertices
        */
        std::vector<std::vector<mesh::Vertex*>> verticesOfFaces();

};

}