#include "face.hpp"
#include "edge.hpp"
#include <vector>
#include <cassert>

int mesh::Face::ID_CPT = 0;

std::vector<mesh::Edge*> mesh::Face::getSurroundingEdges(mesh::Edge* startingEdge){
    std::vector<mesh::Edge*> surEdges;

    mesh::Edge* e0 = startingEdge;
    mesh::Edge* curEdge = e0;

    do{
        surEdges.push_back(curEdge);
        if (curEdge->mFaceLeft->mId == mId){
        	curEdge = curEdge->mEdgeLeftCW;
        }else if (curEdge->mFaceRight->mId == mId){
            curEdge = curEdge->mEdgeRightCW;
        }else{
            assert(false);
        }
    }while(curEdge->mId != e0->mId);

    return surEdges;
}

std::vector<mesh::Edge*> mesh::Face::getSurroundingEdges(){
    // print();
    std::vector<mesh::Edge*> directTurn = getSurroundingEdges(mEdge);
    // print();
    std::vector<mesh::Edge*> reverseTurn = mesh::Edge::getReversedEdges(directTurn);
    // print();
    directTurn.insert( directTurn.end(), reverseTurn.begin(), reverseTurn.end() );
    return directTurn;
}

std::vector<mesh::Vertex*> mesh::Face::getSurroundingVertices(){
    std::vector<mesh::Vertex*> surVertices;
    std::vector<mesh::Edge*>surEdges = getSurroundingEdges(mEdge);
    // print();
    for(int i=0; i<int(surEdges.size()); i++){
        // surEdges[i]->mVertexOrigin->print();
        surVertices.push_back(surEdges[i]->mVertexOrigin);
    }
    return surVertices;
}


mesh::Face* mesh::Face::mergeFace(mesh::Face* face){
    mesh::Face* newFace = new Face();
    newFace->mEdge = mEdge;

    std::vector<mesh::Edge*> surEdgeFace;
    
    // updating edges surrounding current face
    surEdgeFace = getSurroundingEdges();
    // printf("sur edges f1:\n");
    // print();
    for(int i=0; i<int(surEdgeFace.size()); i++){
        // surEdgeFace[i]->print();
        if (surEdgeFace[i]->mFaceLeft->mId == mId) surEdgeFace[i]->mFaceLeft = newFace;
        if (surEdgeFace[i]->mFaceRight->mId == mId) surEdgeFace[i]->mFaceRight = newFace;
    }
    // printf("sur edges f1 done\n");

    // updating edges surrounding the second face
    surEdgeFace = face->getSurroundingEdges();
    // printf("sur edges f2:\n");
    // face->print();
    for(int i=0; i<int(surEdgeFace.size()); i++){
        // surEdgeFace[i]->print();
        if (surEdgeFace[i]->mFaceLeft->mId == face->mId) surEdgeFace[i]->mFaceLeft = newFace;
        if (surEdgeFace[i]->mFaceRight->mId == face->mId) surEdgeFace[i]->mFaceRight = newFace;
    }
    // printf("sur edges f1 done\n");

    return newFace;
}

std::string mesh::Face::toString(){
    char buffer[50];
    sprintf(buffer, "f%d -> e%d, toDel = %d, toMerge = %d", mId, mEdge->mId, mToDelete, mToMerge);
    return buffer;
}

void mesh::Face::print(){
    fprintf(stdout, "%s\n", toString().c_str());
}