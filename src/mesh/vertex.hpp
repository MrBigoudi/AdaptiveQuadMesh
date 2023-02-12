#pragma once

#include "edge.hpp"
#include "vector3.hpp"

namespace mesh{

class Edge;

/**
 * The vertex class from the mesh
*/
class Vertex{

    private:
        /**
         * The id counter
        */
        static int ID_CPT;

    public:
        /**
         * The vertex's id
        */
        int mId = (mesh::Vertex::ID_CPT++);

        /**
         * The vertex coordinates
        */
        maths::Vector3* mCoords;

        /**
         *  One of the surronding edge
        */
        mesh::Edge* mEdge;

        /**
         * Flag to delete this vertex
        */
        bool mToDelete = false;

    public:
        /**
         * A basic constructor
         * @param coords The vertex's coordinates
         * @param edge The edge we'll store
        */
        Vertex(maths::Vector3* coords = nullptr, mesh::Edge* edge = nullptr){
            mCoords = coords;
            mEdge = edge;
        };

        /**
         * Cast a vertex into a printable string
         * @return The vertex as a string
        */
        std::string toString();

        /**
         * Print a vertex
        */
        void print();
};

}