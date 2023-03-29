#pragma once

#include "edge.hpp"
#include "vertex.hpp"
#include "maths.hpp"

#include <vector>
#include <algorithm>

namespace mesh{

class Vertex;

class Edge;

class Face;

/**
 * A structure to represent a diagonal
*/
struct Diagonal{
    /**
     * The diagonal face
    */
    mesh::Face* face;

    /**
     * The first vertex of the diagonal
    */
    mesh::Vertex* v1;

    /**
     * The second vertex of the diagonal
    */
    mesh::Vertex* v2;

    /**
     * The length of the diagonal
    */
    float length;
};

/**
 * The face class from the mesh
*/
class Face{

    public:
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

        /**
         * The face shortest diagonal
        */
        mesh::Diagonal* mDiagonal = nullptr;

        /**
         * If the face needs an update
        */
        bool mToUpdate = false;

        /**
         * The face normal
        */
        maths::Vector3* mNormal;

        /**
         * Fitmaps
        */
        float mSFitmap = 0.0f, mMFitmap = 0.0f;

    public:
        /**
         * Get a list of all edges surrounding the face given the starting point
         * @param startingEdge The edge where to start the loop
        */
        std::vector<mesh::Edge*> getSurroundingEdges(mesh::Edge* startingEdge) const;

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
         * Get a list of all faces surrounding the current face (even if conected by only one vertex)
         * @return The list of all faces arround the face
        */
        std::vector<mesh::Face*> getAllSurroundingFaces() const;

        /**
         * Merge two faces into the current one
         * @param face The face we'll merge
        */
        void mergeFace(mesh::Face* face);

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
         * Get the common edges between two faces
         * @param f2 The second face
         * @return The shared edges between the current face and the given one
        */
        std::vector<mesh::Edge*> getSharedEdges(mesh::Face* f2) const;

        /**
         * Get the list of unconnected vertices between two faces
         * @param f2 The second face
         * @return The list of vertices
        */
        std::vector<mesh::Vertex*> getUnconnectedVertices(mesh::Face* f2) const;

        /**
         * Set the diagonal as the shortest diagonal of a face
        */
        void createDiagonal();

        /**
         * Get a min heap according to the diagonal size from a list of faces
         * @param faces The list of faces
         * @return The diagonal min heap
        */
        static std::vector<mesh::Diagonal*> getMinHeap(std::vector<mesh::Face*> faces);

        /**
         * Compare two diagonals
         * @param d1 The first diagonal
         * @param d2 The second diagonal
         * @return The diagonal with the shortest length
        */
        static mesh::Diagonal* cmpDiagonal(mesh::Diagonal* d1, mesh::Diagonal* d2);

        /**
         * Test if a face is a doublet
         * @return The two edges to remove as a vector
        */
        std::vector<mesh::Edge*> isDoublet() const;

        /**
         * Test if a face is a singlet
         * @return True if it is, false otherwise
        */
        bool isSinglet() const;

        /**
         * Mark a list of faces as to update
         * @param faces The faces to mark
        */
        static void markToUpdate(std::vector<mesh::Face*> faces);

};

}