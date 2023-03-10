#pragma once

#include "edge.hpp"
#include "face.hpp"
#include "vector3.hpp"

namespace mesh{

class Edge;
class Face;

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

        /**
         * Get a common vertices between two faces
         * @param f1 The first face
         * @param f2 The second face
         * @return The vertices in common (or empty list if it doesn't exist)
        */
        static std::vector<mesh::Vertex*> getCommonVertices(mesh::Face* f1, mesh::Face* f2);

        /**
         * Get a common vertex between three faces
         * @param f1 The first face
         * @param f2 The second face
         * @param f3 The third face
         * @return The vertex in common (or nullptr if it doesn't exist)
        */
        static mesh::Vertex* getCommonVertex(mesh::Face* f1, mesh::Face* f2, mesh::Face* f3);


        /**
         * Get the vertex from one face which is isolated from it's 2 given neighbours
         * @param faceToIsolate The face from which we want to find the isolated vertex
         * @param f2 The first neighbour
         * @param f3 The second neighbour
         * @return The wanted vertex (nullptr if it doesn't exist or the first vertex matching the criteria if the faces aren't neighbours of each others)
        */
        static mesh::Vertex* getIsolatedVertex(mesh::Face* faceToIsolate, mesh::Face* f2, mesh::Face* f3);

        /**
         * Test if the vertex is present in a given list of vertices
         * @param list The list of vertices
         * @return True if it is, false if it is not
        */
        bool isInList(std::vector<mesh::Vertex*> list) const;
};

}