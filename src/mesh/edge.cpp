#include "edge.hpp"
#include "vector3.hpp"
#include <stdexcept>
#include <cassert>

int mesh::Edge::ID_CPT = 0;

bool mesh::Edge::isRemovable(){
    return (!this->mFaceLeft->mToMerge) && (!this->mFaceRight->mToMerge); 
}

float mesh::Edge::getSumPairwiseDotProd(std::vector<mesh::Edge*> edgeList){
    std::vector<mesh::Vertex*> vertexList = mesh::Edge::getVertFromSurrEdges(edgeList);
    float sum = 0;

    for(int i=0; i<int(vertexList.size()-1); i++){
        sum += maths::Vector3::dot(vertexList[i]->mCoords, vertexList[i+1]->mCoords);
    }

    return sum;
}

float mesh::Edge::getPerimeter(std::vector<mesh::Edge*> edgeList){
    std::vector<mesh::Vertex*> vertexList = mesh::Edge::getVertFromSurrEdges(edgeList);
    float perimeter = 0;

    for(int i=0; i<int(vertexList.size()-1); i++){
        perimeter += maths::Vector3::distance(vertexList[i]->mCoords, vertexList[i+1]->mCoords);
    }

    return perimeter;
}

float mesh::Edge::getLength() const {
    return maths::Vector3::distance(mVertexOrigin->mCoords, mVertexDestination->mCoords);
}

std::vector<mesh::Vertex*> mesh::Edge::getVertFromSurrEdges(std::vector<mesh::Edge*> edgeList){
    std::vector<mesh::Vertex*> vertexList;

    for(int i=0; i<int(edgeList.size()); i++){
        vertexList.push_back(edgeList[i]->mVertexOrigin);
    }
    // adding first vertex to have a loop
    vertexList.push_back(edgeList[0]->mVertexOrigin);

    return vertexList;
}

mesh::EdgePos mesh::Edge::getEdgePos(mesh::Edge* edgeLookingFor){

    // printf("\nthis: %p, toCheck: %p\n", this, edgeLookingFor);
    // printf("lcw: %p, lccw: %p\n", mEdgeLeftCW, mEdgeLeftCCW);
    // printf("rcw: %p, rccw: %p\n\n", mEdgeRightCW, mEdgeRightCCW);
    if(edgeLookingFor == mEdgeLeftCW)   return LCW;
    if(edgeLookingFor == mEdgeLeftCCW)  return LCCW;
    if(edgeLookingFor == mEdgeRightCW)  return RCW;
    if(edgeLookingFor == mEdgeRightCCW) return RCCW;
    return NONE;

    // std::fprintf(stderr, "Error, failed to look up for an edge!\n");
    // throw std::invalid_argument("The given edge is not a neighbour of the current one!\n");
}


std::string mesh::Edge::toString(){
    char buffer[256];
    sprintf(buffer, "e%d -> lcw%d, lccw%d, rcw%d, rccw%d, vo%d, vd%d, fl%d, fr%d, rev%d, toDel = %d", 
            mId, mEdgeLeftCW->mId, mEdgeLeftCCW->mId, mEdgeRightCW->mId, mEdgeRightCCW->mId, 
            mVertexOrigin->mId, mVertexDestination->mId, mFaceLeft->mId, mFaceRight->mId, mReverseEdge->mId, mToDelete);
    return buffer;
}

void mesh::Edge::print(){
    fprintf(stdout, "%s\n", toString().c_str());
}

std::vector<mesh::Edge*> mesh::Edge::getReversedEdges(std::vector<mesh::Edge*> edgeList){
    std::vector<mesh::Edge*> reversedEdges;
    for (int i=0; i<int(edgeList.size()); i++){
        reversedEdges.push_back(edgeList[i]->mReverseEdge);
    }
    return reversedEdges;
}


void mesh::Edge::updateAllNeighbours(){
    if(mEdgeLeftCCW->mEdgeLeftCW->mId == mId)
        mEdgeLeftCCW->mEdgeLeftCW = mEdgeRightCW->mReverseEdge;
    if(mEdgeLeftCW->mEdgeLeftCCW->mId == mId)
        mEdgeLeftCW->mEdgeLeftCCW = mEdgeRightCCW->mReverseEdge;
    if(mEdgeRightCCW->mEdgeRightCW->mId == mId)
        mEdgeRightCCW->mEdgeRightCW = mEdgeLeftCW->mReverseEdge;
    if(mEdgeRightCW->mEdgeRightCCW->mId == mId)
        mEdgeRightCW->mEdgeRightCCW = mEdgeLeftCCW->mReverseEdge;
}

void mesh::Edge::createReversed(mesh::Edge* edge){
    edge->mVertexOrigin = mVertexDestination;
    edge->mVertexDestination = mVertexOrigin;
    edge->mFaceLeft = mFaceRight;
    edge->mFaceRight = mFaceLeft;
    edge->mEdgeRightCCW = mEdgeLeftCCW->mReverseEdge;
    edge->mEdgeRightCW = mEdgeLeftCW->mReverseEdge;
    edge->mEdgeLeftCCW = mEdgeRightCCW->mReverseEdge;
    edge->mEdgeLeftCW = mEdgeRightCW->mReverseEdge;
}


bool mesh::Edge::cmp(const mesh::Edge &e1, const mesh::Edge &e2){
    return e1.mSumDotProd < e2.mSumDotProd;
}
