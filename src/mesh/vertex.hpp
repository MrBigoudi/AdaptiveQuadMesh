#pragma once

#include "edge.hpp"
#include "vector3.hpp"

namespace mesh{

class Edge;

/**
 * The vertex class from the mesh
*/
class Vertex{

    public:
        /**
         * The vertex index
        */
        int mId;

        /**
         * The vertex coordinates
        */
        maths::Vector3* mCoords;

        /**
         *  One of the surronding edge
        */
        mesh::Edge* mEdge;

    public:
        /**
         * A basic constructor
         * @param id The vertex's index
         * @param coords The vertex's coordinates
         * @param edge The edge we'll store
        */
        Vertex(int id, maths::Vector3* coords = nullptr, mesh::Edge* edge = nullptr){
            mId = id;
            mCoords = coords;
            mEdge = edge;
        };
};

}