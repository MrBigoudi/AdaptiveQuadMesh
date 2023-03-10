#include "face.hpp"
#include "edge.hpp"
#include <vector>
#include <cassert>

int mesh::Face::ID_CPT = 0;

std::vector<mesh::Edge*> mesh::Face::getSurroundingEdges(mesh::Edge* startingEdge) const{
    std::vector<mesh::Edge*> surEdges;

    mesh::Edge* e0 = startingEdge;
    mesh::Edge* curEdge = e0;

    // printf("\n\nBeg Do While:\ne0: %d\n", e0->mId);
    do{
        // curEdge->print();
        surEdges.push_back(curEdge);
        // printf("curEdge: %d, e0: %d\n", curEdge->mId, e0->mId);
        if (curEdge->mFaceLeft->mId == mId){
        	curEdge = curEdge->mEdgeLeftCW;
        }else if (curEdge->mFaceRight->mId == mId){
            curEdge = curEdge->mEdgeRightCW;
        }else{
            assert(false);
        }
    }while(curEdge->mId != e0->mId);
    // printf("\n\nEnd Do While:\n\n\n\n");

    return surEdges;
}

std::vector<mesh::Edge*> mesh::Face::getSurroundingEdges() const{
    // print();
    std::vector<mesh::Edge*> directTurn = getSurroundingEdges(mEdge);
    // print();
    std::vector<mesh::Edge*> reverseTurn = mesh::Edge::getReversedEdges(directTurn);
    // print();
    directTurn.insert( directTurn.end(), reverseTurn.begin(), reverseTurn.end() );
    return directTurn;
}

std::vector<mesh::Vertex*> mesh::Face::getSurroundingVertices() const{
    std::vector<mesh::Vertex*> surVertices;
    std::vector<mesh::Edge*>surEdges = getSurroundingEdges(mEdge);
    // print();
    for(int i=0; i<int(surEdges.size()); i++){
        // surEdges[i]->mVertexOrigin->print();
        surVertices.push_back(surEdges[i]->mVertexOrigin);
    }
    return surVertices;
}


std::vector<mesh::Face*> mesh::Face::getSurroundingFaces() const{
    std::vector<mesh::Face*> surFaces;
    std::vector<mesh::Edge*>surEdges = getSurroundingEdges(mEdge);
    // print();
    for(int i=0; i<int(surEdges.size()); i++){
        if(surEdges[i]->mFaceRight->mId != mId)
            surFaces.push_back(surEdges[i]->mFaceRight);
    }
    return surFaces;
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
    newFace->mIsTriangle = false;

    return newFace;
}

std::string mesh::Face::toString() const{
    char buffer[50];
    sprintf(buffer, "f%d -> e%d, toDel = %d, toMerge = %d, isTriangle: %d", mId, mEdge->mId, mToDelete, mToMerge, mIsTriangle);
    return buffer;
}

void mesh::Face::print() const{
    fprintf(stdout, "%s\n", toString().c_str());
}

bool mesh::Face::isTriangle() const{
    return mIsTriangle;
}

bool mesh::Face::isQuad() const {
    return !isTriangle();
}

// bool mesh::Face::isDividable() const {
//     // all surrounding faces are triangle
//     std::vector<mesh::Face*> surFaces = getSurroundingFaces();
//     for(int i=0; i<int(surFaces.size()); i++){
//         if(!surFaces[i]->isTriangle())
//             return false;
//     }
//     return true;
// }

mesh::Edge* mesh::Face::getEdgeBetween(mesh::Face* face) const{
    std::vector<mesh::Edge*> surEdges = getSurroundingEdges();
    int f2Id = face->mId;

    // printf("\n\nEdge between:\nface:\n");
    // face->print();

    for(int i=0; i<int(surEdges.size()); i++){
        mesh::Edge* curEdge = surEdges[i];
        // printf("\ncurEdge:\n");
        // curEdge->print();

        if( (curEdge->mFaceLeft->mId == mId && curEdge->mFaceRight->mId == f2Id)
            || (curEdge->mFaceLeft->mId == f2Id && curEdge->mFaceRight->mId == mId)){
                return curEdge;
        }
    }

    return nullptr;
}
