#pragma once

#include <vector>
#include <string>

#include "face.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "vector3.hpp"

namespace mesh{

class Vertex;
class Face;
class Edge;

/**
 * The mesh class using winged mesh representation
*/
class Mesh{

    public:
        /**
         *  The number of vertices in the mesh
        */
        int mNbVertices;

        /**
         * The number of faces in the mesh
        */
        int mNbFaces;

        /**
         * The number of edges in the mesh
        */
        int mNbEdges;

        /**
         * The mesh's vertices
        */
        std::vector<mesh::Vertex*> mVertices;

        /**
         * The mesh's faces
        */
        std::vector<mesh::Face*> mFaces;

        /**
         * The mesh's edges
        */
        std::vector<mesh::Edge*> mEdges;

    public:

        /**
         * The basic constructor
         * @param nbVertices The number of vertices in the mesh
         * @param nbFaces The number of faces in the mesh
         * @param nbEdges The number of edges in the mesh
         * @param vertices The mesh's vertices
         * @param faces The mesh's faces
         * @param edges The mesh's edges
        */
        Mesh(int nbVertices, int nbFaces, int nbEdges, 
                std::vector<mesh::Vertex*> vertices, 
                std::vector<mesh::Face*> faces, 
                std::vector<mesh::Edge*> edges){
            mNbVertices = nbVertices;
            mNbFaces = nbFaces;
            mNbEdges = nbEdges;
            mVertices = vertices;
            mFaces = faces;
            mEdges = edges;
        };

        /**
         * The destructor
        */
        ~Mesh(){};

        /**
         * Creat a mesh from an object file
         * @param file A file containing the mesh representation
         * @exception Invalid_Argument if the file is not correct
         * @return A new mesh
        */
        static Mesh loadOBJ(std::string file);

        /**
         * Create an obj file from a mesh
         * @param file The produced file
         * @exception Invalid_Argument if the file is not correct
        */
        void toObj(std::string file);

        /**
         * Transform a triangular mesh into a quad one
        */
        void triToQuad();

        /**
         * Cast the mesh into a list of printable strings
         * @return The mesh as a string vector
        */
        std::vector<std::string> toString();

        /**
         * Cast the mesh's vertices into a list of printable strings
         * @return The vertices as a string vector
        */
        std::vector<std::string> verticesToString();

        /**
         * Cast the mesh's faces into a list of printable strings
         * @return The faces as a string vector
        */
        std::vector<std::string> facesToString();

        /**
         * Cast the mesh's edges into a list of printable strings
         * @return The edges as a string vector
        */
        std::vector<std::string> edgesToString();

        /**
         * Print the mesh in the standard output
        */
        void print();


    private:
        /**
         * Create a mesh from vertices and faces of an obj file
         * @param vertices The vertices from the obj file
         * @param faces The faces from the obj file
         * @return A new mesh
        */
        static mesh::Mesh objToMesh(std::vector<maths::Vector3*> &vertices, std::vector<std::vector<int>> &faces);

        /**
         * Extract the vertices from the mesh's faces
         * @return A matrix of vertices
        */
        std::vector<std::vector<mesh::Vertex*>> verticesOfFaces();

        /**
         * Mark the edges to remove to transform a triangular mesh into a quad one
        */
        void triToQuadRemovalMarkingPhase();

        /**
         * Transform a quad-dominant mesh into a real triangular mesh
        */
	    void triToPureQuad();

        /**
         * Remove all marked edges
        */
        void removeMarkedEdges();

        /**
         * Remove an edge
         * @param edge The edge to be removed from the mesh
        */
        void removeEdge(mesh::Edge* edge);

        /**
         * Remove a face from the list
         * @param face The face to remove
        */
        void removeFaceFromList(mesh::Face* face);

        /**
         * Remove an edge from the list
         * @param edge The edge to remove
        */
        void removeEdgeFromList(mesh::Edge* edge);

        /**
         * Get a triangle face from the mesh
         * @return The triangle or nullptr if there are none
        */
        mesh::Face* getTriangle();

        /**
         * Count the number of triangles in the mesh
         * @return The number of triangles
        */
        int howManyTriangles();

        /**
         * Transform a triangle into 3 quads
         * @param triangle The triangle to change
        */
        void subdivideTriangle(mesh::Face* triangle);

        /**
         * Create the new elements for subdividing a triangle
         * @param triangle The triangle we want to remove
         * @param newVertices A reference to the newly created vertices
         * @param newEdges A reference to the newly created edges
         * @param newFaces A reference to the newly created faces
        */
        void createQuads(mesh::Face* triangle, 
            std::vector<mesh::Vertex*> &newVertices, 
            std::vector<mesh::Edge*> &newEdges, 
            std::vector<mesh::Face*> & newFaces
        );

        /**
         * Initialize the new elements for subdividing a triangle
         * @param triangle The triangle we want to remove
         * @param newVertices A reference to the newly created vertices
         * @param newEdges A reference to the newly created edges
         * @param newFaces A reference to the newly created faces
        */
        void initQuads(mesh::Face* triangle,
            std::vector<mesh::Vertex*> &newVertices, 
            std::vector<mesh::Edge*> &newEdges, 
            std::vector<mesh::Face*> & newFaces
        );

        /**
         * Initiate the edges arround the triangle
         * @param newVertices A reference to the newly created vertices
         * @param newEdges A reference to the newly created edges
         * @param newFaces A reference to the newly created faces
         * @param oldVertices A reference to the old vertices surrounding the triangle
         * @param oldEdges A reference to the old edges surrounding the triangle
        */
        static void initEdgesArroundTriFace(
            std::vector<mesh::Vertex*> &newVertices, 
            std::vector<mesh::Edge*> &newEdges, 
            std::vector<mesh::Face*> & newFaces,
            std::vector<mesh::Vertex*> & oldVertices,
            std::vector<mesh::Edge*> & oldEdges);

        /**
         * Initiate the edges inside the triangle
         * @param newVertices A reference to the newly created vertices
         * @param newEdges A reference to the newly created edges
         * @param newFaces A reference to the newly created faces
        */
        static void initEdgesInsideTriFace(
            std::vector<mesh::Vertex*> &newVertices, 
            std::vector<mesh::Edge*> &newEdges, 
            std::vector<mesh::Face*> & newFaces);

        /**
         * Initiate the reversed edges of the triangle
         * @param newEdges A reference to the newly created edges
        */
        static void initReversedEdgesTriFace(std::vector<mesh::Edge*> &newEdges);


        /**
         * Get the list of all edges to delete
         * @return The edges as a vector list
        */
        std::vector<mesh::Edge*> getAllEdgesToDelete();

};

}