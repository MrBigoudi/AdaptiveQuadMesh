#pragma once

#include "edge.hpp"

namespace mesh{

class Edge;

/**
 * The face class from the mesh
*/
class Face{

    public:
        /**
         *  One of the surronding edge
        */
        mesh::Edge* mEdge;

    public:
        /**
         * A basic constructor
         * @param edge The edge we'll store
        */
        Face(mesh::Edge* edge = nullptr){
            mEdge = edge;
        };
};

}