#pragma once

#include "face.hpp"
#include "vertex.hpp"

namespace mesh{

class Face;
class Vertex;

/**
 * The edge class from the mesh
*/
class Edge{

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


    public:
        /**
         * A basic constructor
         * @param 
        */
        Edge(mesh::Vertex* vOrigin = nullptr, mesh::Vertex* vDest = nullptr, mesh::Face* faceL = nullptr, mesh::Face* faceR = nullptr, 
                mesh::Edge* edgeLCW = nullptr, mesh::Edge* edgeLCCW = nullptr, mesh::Edge* edgeRCW = nullptr, mesh::Edge* edgeRCCW = nullptr){
            mVertexOrigin = vOrigin;
            mVertexDestination = vDest;
            mFaceLeft = faceL;
            mFaceRight = faceR;
            mEdgeLeftCW = edgeLCW;
            mEdgeLeftCCW = edgeLCCW;
            mEdgeRightCW = edgeRCW;
            mEdgeRightCCW = edgeRCCW;
        };
};

}