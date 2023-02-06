#pragma once

#include "edge.hpp"

namespace mesh{

/**
 * The vertex class from the mesh
*/
class Vertex{

    private:

        /**
         * The vertex coordinates
        */


        /**
         *  One of the surronding edge
        */
        mesh::Edge mEdge;

    public:

        /**
         * A basic constructor
         * @param edge The edge we'll store
        */
        Vertex(mesh::Edge & edge){
            mEdge = edge;
        };
};

}