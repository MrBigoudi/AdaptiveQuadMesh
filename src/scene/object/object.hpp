#pragma once

#include <vector>
#include <string>

#include "mesh.hpp"

namespace scene{

/**
 * A class to manipulate the objects plotted on screen
*/
class Object{

    public:
        /**
         * The list of vertices to draw
        */
        std::vector<float> mVertices;

        /**
         * The list of indices
        */
        std::vector<unsigned int> mIndices;

        /**
         * The mesh
        */
        mesh::Mesh* mMesh;


    public:
        /**
         * A constructor taking a file path as input
         * @param obj The file to the object file we'll use to init  
        */
        Object(std::string obj);

        /**
         * Transform the mesh into a quad one
        */
        void toQuadMesh();

        /**
         * Get the vertices
         * @return The object's vertices
        */
        const float* getVertices() const;

        /**
         * Get the indices
         * @return The object's indices
        */
        const unsigned int* getIndices() const;

        /**
         * Get the number of vertices
         * @return The number of vertices
        */
        unsigned int nbVertices() const;

        /**
         * Get the number of indices
         * @return The number of indices
        */
        unsigned int nbIndices() const;

        /**
         * Create an obj file from the object
         * @param file The produced file
         * @exception Invalid_Argument if the file is not correct
        */
        void toObj(std::string file);

    private:
        /**
         * Transform the vertices and the faces of the mesh into vertices
        */
        void initVerticesAndIndices();

};

}