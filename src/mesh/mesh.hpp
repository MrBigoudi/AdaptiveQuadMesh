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
         * Check mesh correctness
        */
        void checkCorrectness() const;

        /**
         * Transform a triangular mesh into a quad one
        */
        void triToQuad();

        /**
         * Cast the mesh into a list of printable strings
         * @return The mesh as a string vector
        */
        std::vector<std::string> toString() const;

        /**
         * Cast the mesh's vertices into a list of printable strings
         * @return The vertices as a string vector
        */
        std::vector<std::string> verticesToString() const;

        /**
         * Cast the mesh's faces into a list of printable strings
         * @return The faces as a string vector
        */
        std::vector<std::string> facesToString() const;

        /**
         * Cast the mesh's edges into a list of printable strings
         * @return The edges as a string vector
        */
        std::vector<std::string> edgesToString() const;

        /**
         * Print the mesh in the standard output
        */
        void print() const;

        /**
         * Print the mesh's stats in the standard output
        */
        void printStats() const;


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

        // /**
        //  * Mark the edges to delete to transform a quad-dominant mesh into a real triangular mesh, 
        //  * using catmull-clark subdivision on the remaining triangles
        // */
	    // void triToPureQuadMarkingPhase();

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

        // /**
        //  * Get the list of triangle faces from the mesh
        //  * @return The triangles as a list
        // */
        // std::vector<mesh::Face*> getTriangles();

        /**
         * Get one of the remaining triangles in the mesh
         * @return The face of the triangle (nullptr if there are no triangles left)
        */
        mesh::Face* getTriangle() const;

        /**
         * Count the number of triangles in the mesh
         * @return The number of triangles
        */
        int howManyTriangles() const;

        /**
         * Transform a quad dominant mesh into a pure quad one
        */
        void triToPureQuad();

        /**
         * Get the path of faces to reach the closest triangle using a BFS
         * @param triOrigin The triangle from where to start
         * @return The path as a list of faces (assert false if triOrigin is the last triangle)
        */
        std::vector<mesh::Face*> pathToClosestTriangle(mesh::Face* triangle) const;

        /**
         * Create an edge in separating a given faace through tow given vertices
         * @param face The face to split
         * @param v1 The first vertex the new edge will go through
         * @param v2 The second vertex the new edge will go through
        */
        void createEdge(mesh::Face* face, mesh::Vertex* v1, mesh::Vertex* v2);

        // /**
        //  * Transform a triangle into 3 quads
        //  * @param triangle The triangle to change
        // */
        // void subdivideTriangle(mesh::Face* triangle);

        // /**
        //  * Create the new elements for subdividing a triangle
        //  * @param triangle The triangle we want to remove
        //  * @param newVertices A reference to the newly created vertices
        //  * @param newEdges A reference to the newly created edges
        //  * @param newFaces A reference to the newly created faces
        // */
        // void createQuads(mesh::Face* triangle, 
        //     std::vector<mesh::Vertex*> &newVertices, 
        //     std::vector<mesh::Edge*> &newEdges, 
        //     std::vector<mesh::Face*> & newFaces
        // );

        // /**
        //  * Initialize the new elements for subdividing a triangle
        //  * @param triangle The triangle we want to remove
        //  * @param newVertices A reference to the newly created vertices
        //  * @param newEdges A reference to the newly created edges
        //  * @param newFaces A reference to the newly created faces
        // */
        // void initQuads(mesh::Face* triangle,
        //     std::vector<mesh::Vertex*> &newVertices, 
        //     std::vector<mesh::Edge*> &newEdges, 
        //     std::vector<mesh::Face*> & newFaces
        // );

        // /**
        //  * Initiate the edges arround the triangle
        //  * @param newVertices A reference to the newly created vertices
        //  * @param newEdges A reference to the newly created edges
        //  * @param newFaces A reference to the newly created faces
        //  * @param oldVertices A reference to the old vertices surrounding the triangle
        //  * @param oldEdges A reference to the old edges surrounding the triangle
        // */
        // static void initEdgesArroundTriFace(
        //     std::vector<mesh::Vertex*> &newVertices, 
        //     std::vector<mesh::Edge*> &newEdges, 
        //     std::vector<mesh::Face*> & newFaces,
        //     std::vector<mesh::Vertex*> & oldVertices,
        //     std::vector<mesh::Edge*> & oldEdges);

        // /**
        //  * Initiate the edges inside the triangle
        //  * @param newVertices A reference to the newly created vertices
        //  * @param newEdges A reference to the newly created edges
        //  * @param newFaces A reference to the newly created faces
        // */
        // static void initEdgesInsideTriFace(
        //     std::vector<mesh::Vertex*> &newVertices, 
        //     std::vector<mesh::Edge*> &newEdges, 
        //     std::vector<mesh::Face*> & newFaces);

        // /**
        //  * Initiate the reversed edges of the triangle
        //  * @param newEdges A reference to the newly created edges
        // */
        // static void initReversedEdgesTriFace(std::vector<mesh::Edge*> &newEdges);


        /**
         * Get the list of all edges to delete
         * @return The edges as a vector list
        */
        std::vector<mesh::Edge*> getAllEdgesToDelete();

        // /**
        //  * Test if the possible edges have already been visited
        //  * @param face The face as a list of integers (willl be modified)
        //  * @param reversed Tells if the rotation directioin have been reversed
        //  * @param edgeTable The list of already visited edges
        //  * @param position The index of the currently tried position
        //  * @return True if at least one of the edges has already been visited
        // */
        // static bool edgesAlreadyVisited(std::vector<int> & face, bool & reversed, const std::vector<std::vector<int>> & edgeTable, int position);

        // /**
        //  * Create the middle vertex for the Catmull-Clark subdivision of remaining triangles
        //  * @param triangle The old triangle to remove
        //  * @return A new vertex at the barycenter of old triangle
        // */
        // mesh::Vertex* triToQuadNewMiddleVertex(mesh::Face* triangle);

        // /**
        //  * Create the new faces for the Catmull-Clark subdivision of remaining triangles
        //  * @param triangle The old triangle to remove
        //  * @return A list of newly created faces
        // */
        // std::vector<mesh::Face*> triToQuadNewFaces(mesh::Face* triangle);

        // /**
        //  * Create the new edges for the Catmull-Clark subdivision of remaining triangles
        //  * @param triangle The old triangle to remove
        //  * @param midVertex The middle vertex created at the center of the old triangle
        //  * @return A list of newly created edges
        // */
        // std::vector<mesh::Edge*> triToQuadNewEdges(mesh::Face* triangle, mesh::Vertex* midVertex);

        // /**
        //  * Remove the old triangle from the mesh and updates old neighbours
        //  * @param triangle The triangle to remove
        // */
        // void triToQuadRemoveOldTriangle(mesh::Face* triangle);

};

}