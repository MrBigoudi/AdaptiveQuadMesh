#pragma once

#include <vector>
#include <string>

#include "face.hpp"
#include "vertex.hpp"

namespace mesh{

class Face;
class Vertex;

enum EdgePos {LCW, LCCW, RCW, RCCW, NONE};

/**
 * The edge class from the mesh
*/
class Edge{

    private:
        /**
         * The id counter
        */
        static int ID_CPT;

    public:
        /**
         * First vertex of the edge
        */
        mesh::Vertex* mVertexOrigin;
        
        /**
         * First vertex of the edge
        */
        mesh::Vertex* mVertexDestination;

        /**
         * Left face
        */
        mesh::Face* mFaceLeft; 
        
        /**
         * Right face
        */
        mesh::Face* mFaceRight;

        /**
         * Clock wise left edge
        */
        mesh::Edge* mEdgeLeftCW;

        /**
         * Counter clock wise left edge
        */
        mesh::Edge* mEdgeLeftCCW;

        /**
         * Clock wise right edge
        */
        mesh::Edge* mEdgeRightCW;

        /**
         * Counter clock wise right edge
        */
        mesh::Edge* mEdgeRightCCW;

        /**
         * The reversed edge
        */
        mesh::Edge* mReverseEdge;

        /**
         * Flag to delete this edge
        */
        bool mToDelete = false;

        /**
         * The edge's id
        */
        int mId = (mesh::Edge::ID_CPT++);

        /**
         * The sum of pariwised dot product
        */
        float mSumDotProd;


    public:
        /**
         * A basic constructor
         * @param 
        */
        Edge(mesh::Vertex* vOrigin = nullptr, mesh::Vertex* vDest = nullptr, mesh::Face* faceL = nullptr, mesh::Face* faceR = nullptr, 
                mesh::Edge* edgeLCW = nullptr, mesh::Edge* edgeLCCW = nullptr, mesh::Edge* edgeRCW = nullptr, mesh::Edge* edgeRCCW = nullptr, 
                    mesh::Edge* reversed = nullptr){
            mVertexOrigin = vOrigin;
            mVertexDestination = vDest;
            mFaceLeft = faceL;
            mFaceRight = faceR;
            mEdgeLeftCW = edgeLCW;
            mEdgeLeftCCW = edgeLCCW;
            mEdgeRightCW = edgeRCW;
            mEdgeRightCCW = edgeRCCW;
            mReverseEdge = reversed;
        };

        /**
         * A constructor by copy
         * @param edge The edge we'll copy
        */
        Edge(mesh::Edge* edge){
            mVertexOrigin = edge->mVertexOrigin;
            mVertexDestination = edge->mVertexDestination;
            mFaceLeft = edge->mFaceLeft;
            mFaceRight = edge->mFaceRight;
            mEdgeLeftCW = edge->mEdgeLeftCW;
            mEdgeLeftCCW = edge->mEdgeLeftCCW;
            mEdgeRightCW = edge->mEdgeRightCW;
            mEdgeRightCCW = edge->mEdgeRightCCW;
            mReverseEdge = edge->mReverseEdge;
        }

        /**
         * Check if the edge is removable or not
         * @return True if this edge can be removed, false otherwise
        */
        bool isRemovable();

        /**
         * Check if a list of edges has doubles
         * @param edges The list of edges to check
         * @return True if it has doubles
        */
        static bool hasDoubles(std::vector<mesh::Edge*> edges);

        /**
         * Get the sum of pairwise dot products of edges surrounding a face
         * @param vertexList A list of vertices surrounding a face 
         * @return The sum of the dot products
        */
        static float getSumPairwiseDotProd(std::vector<mesh::Vertex*> vertexList);

        /**
         * Get the perimeter of edges surrounding a face
         * @param edgeList A list of chained edges surrounding a face 
         * @return The perimeter of the face
        */
        static float getPerimeter(std::vector<mesh::Edge*> edgeList);

        /**
         * Get the edge position of one edge compare to another
         * @param edgeLookingFor The edge for which we want to find the position
         * @return The position of the edge we're looking for within the current edge
        */
        mesh::EdgePos getEdgePos(mesh::Edge* edgeLookingFor);

        /**
         * Get the length of an edge
         * @return The length as a float
        */
        float getLength() const;

        /**
         * Cast an edge into a printable string
         * @return The edge as a string
        */
        std::string toString();

        /**
         * Print an edge
        */
        void print();

        /**
         * Get the list of all reversed edges of a given list of edges
         * @param edgeList The edges for which we want the reversed
         * @return A list of the reversed edges
        */
        static std::vector<mesh::Edge*> getReversedEdges(std::vector<mesh::Edge*> edgeList);

        /**
         * Update all the edges' neighbours before its removal
        */
        void updateAllNeighbours();

        /**
         * Inititate a given edge as the reversed of the current one
         * @param edge The edge to initiate
        */
        void createReversed(mesh::Edge* edge);

        /**
         * Compare two edges in term of their pairwised dot product sum
         * @param e1 The first edge
         * @param e2 The second edge
         * @return True if the first edge is lesser than the second one
        */
        static bool cmp(const mesh::Edge &e1, const mesh::Edge &e2);

        /**
         * Merge two edges
         * @param e2 The second edge
        */
        void mergeEdge(mesh::Edge* e2);

    private:
        /**
         * Get a list of all the vertices from edges surrounding a face
         * @param edgeList A list of chained edges surrounding a face 
         * @return The list of the vertices the edges go through
        */
        static std::vector<mesh::Vertex*> getVertFromSurrEdges(std::vector<mesh::Edge*> edgeList);
};

}