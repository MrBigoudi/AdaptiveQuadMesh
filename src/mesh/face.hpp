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

        /**
         * Flag to know it the face is a triangle
        */
        bool mIsTriangle = true;

    private:
        /**
         * Get a list of all edges surrounding the face given the starting point
         * @param startingEdge The edge where to start the loop
        */
        std::vector<mesh::Edge*> getSurroundingEdges(mesh::Edge* startingEdge) const;

    public:
        /**
         * A basic constructor
         * @param edge The edge we'll store
        */
        Face(mesh::Edge* edge = nullptr){
            mEdge = edge;
        };

        /**
         * Get a list of all edges surrounding the current face
         * @return The list of all the edges arround the face (including reversed edges)
        */
        std::vector<mesh::Edge*> getSurroundingEdges() const;

        /**
         * Get a list of all vertices surrounding the current face
         * @return The list of all vertices arround the face
        */
        std::vector<mesh::Vertex*> getSurroundingVertices() const;

        /**
         * Get a list of all faces surrounding the current face
         * @return The list of all faces arround the face
        */
        std::vector<mesh::Face*> getSurroundingFaces() const;

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
        std::string toString() const;

        /**
         * Print a face
        */
        void print() const;

        /**
         * Check if the face is a triangle
         * @return True if it is a triangle, false otherwise
        */
        bool isTriangle() const;

        /**
         * Check if the face is a quad
         * @return True if it is a quad, false otherwise
        */
        bool isQuad() const;

        // /**
        //  * Check if the face is dividable into a quad without causing surrounding faces to become more than quads
        //  * @return True if it is a dividable, false otherwise
        // */
        // bool isDividable() const;

        /**
         * Get the edge between two given faces
         * @param face The second face
         * @return The edge between the current face and the given one (nullptr if no such edge exists)
        */
        mesh::Edge* getEdgeBetween(mesh::Face* face) const;

        /**
         * Give the number of common edges between two faces
         * @param f2 The second face
         * @return The number of shared edges between the current face and the given one
        */
        int getNumberOfSharedEdges(mesh::Face* f2) const;

        /**
         * Get the list of unconnected vertices between two faces
         * @param f2 The second face
         * @return The list of vertices
        */
        std::vector<mesh::Vertex*> getUnconnectedVertices(mesh::Face* f2) const;

};

}