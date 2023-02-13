#pragma once

#include "edge.hpp"
#include "vertex.hpp"
#include <vector>

namespace mesh{

class Vertex;

class Edge;

/**
 * The face class from the mesh
*/
class Face{

    private:
        /**
         * The id counter
        */
        static int ID_CPT;

    public:
        /**
         *  One of the surronding edge
        */
        mesh::Edge* mEdge;

        /**
         * Flag to delete this face
        */
        bool mToDelete = false;

        /**
         * Flag to know it this face will be merge
        */
        bool mToMerge = false;

        /**
         * The face's id
        */
        int mId = (mesh::Face::ID_CPT++);

    private:
        /**
         * Get a list of all edges surrounding the face given the starting point
         * @param startingEdge The edge where to start the loop
        */
        std::vector<mesh::Edge*> getSurroundingEdges(mesh::Edge* startingEdge);

    public:
        /**
         * A basic constructor
         * @param edge The edge we'll store
        */
        Face(mesh::Edge* edge = nullptr){
            mEdge = edge;
        };

        /**
         * Get a list of all edges surrounding the face
         * @return The list of all the edges arround the face (including reversed edges)
        */
        std::vector<mesh::Edge*> getSurroundingEdges();

        /**
         * Get a list of all vertices surrounding the face
         * @return The list of all vertices arround the face
        */
        std::vector<mesh::Vertex*> getSurroundingVertices();

        /**
         * Merge two faces
         * @param face The face we'll merge
         * @return The newly created face
        */
        mesh::Face* mergeFace(mesh::Face* face);

        /**
         * Cast a face into a printable string
         * @return The face as a string
        */
        std::string toString();

        /**
         * Print a face
        */
        void print();

};

}