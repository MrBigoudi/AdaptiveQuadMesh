#pragma once

#include "edge.hpp"

namespace mesh{

/**
 * The face class from the mesh
*/
class Face{

    private:
        /**
         *  One of the surronding edge
        */
        mesh::Edge mEdge;

    public:

        /**
         * A basic constructor
         * @param edge The edge we'll store
        */
        Face(mesh::Edge & edge){
            mEdge = edge;
        };
};

}