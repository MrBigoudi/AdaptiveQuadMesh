#include "face.hpp"
#include "edge.hpp"
#include <vector>
#include <cassert>

int mesh::Face::ID_CPT = 0;

std::vector<mesh::Edge*> mesh::Face::getSurroundingEdges(mesh::Edge* startingEdge) const{
    std::vector<mesh::Edge*> surEdges;

    mesh::Edge* e0 = startingEdge;

    if (e0->mFaceLeft->mId == mId) {
        e0 = e0->mReverseEdge;
        assert(false);
    }

    mesh::Edge* curEdge = e0;

    // printf("\n\nBeg Do While:\ne0: %d\n", e0->mId);
    do{
        // curEdge->print();
        surEdges.push_back(curEdge);
        if (curEdge->mFaceRight->mId == mId){
        	curEdge = curEdge->mEdgeRightCW;
        }else {
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
        if(surEdges[i]->mFaceLeft->mId != mId){
            bool alreadyAdded = false;
            for(int j=0; j<int(surFaces.size()); j++){
                if(surFaces[j]->mId == surEdges[i]->mFaceLeft->mId){
                    alreadyAdded = true;
                    break;
                }
            }
            if(!alreadyAdded)
                surFaces.push_back(surEdges[i]->mFaceLeft);
        }
    }
    return surFaces;
}


void mesh::Face::mergeFace(mesh::Face* face){
    std::vector<mesh::Edge*> surEdgeFace;

    // updating edges surrounding the second face
    surEdgeFace = face->getSurroundingEdges();
    // printf("sur edges f2:\n");
    // face->print();
    for(int i=0; i<int(surEdgeFace.size()); i++){
        // surEdgeFace[i]->print();
        if (surEdgeFace[i]->mFaceLeft->mId == face->mId) surEdgeFace[i]->mFaceLeft = this;
        if (surEdgeFace[i]->mFaceRight->mId == face->mId) surEdgeFace[i]->mFaceRight = this;
    }
    // printf("sur edges f1 done\n");
    mIsTriangle = false;
}

std::string mesh::Face::toString() const{
    char buffer[128];
    sprintf(buffer, "f%d -> e%d, v: %s, toDel = %d, toMerge = %d, isTriangle: %d", mId, mEdge->mId, mesh::Vertex::listToString(getSurroundingVertices()).c_str(), mToDelete, mToMerge, mIsTriangle);
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

int mesh::Face::getNumberOfSharedEdges(mesh::Face* f2) const {
    std::vector<mesh::Edge*> sur1 = getSurroundingEdges();
    std::vector<mesh::Edge*> sur2 = f2->getSurroundingEdges();

    int sum = 0;

    for(int i=0; i<int(sur1.size()); i++){
        for(int j=0; j<int(sur2.size()); j++){
            if(sur1[i]->mId == sur2[j]->mId) sum++;
        }
    }

    return sum;
}

std::vector<mesh::Vertex*> mesh::Face::getUnconnectedVertices(mesh::Face* f2) const{
    std::vector<mesh::Edge*> sur1 = getSurroundingEdges();
    std::vector<mesh::Edge*> sur2 = f2->getSurroundingEdges();

    std::vector<mesh::Vertex*> unconnected;
    bool exists = false;
    mesh::Vertex* curVertex;
    mesh::Vertex* nextVertex;

    if(sur1.size() >= sur2.size()){
        for(int i=0; i<int(sur1.size()); i++){
            curVertex = sur1[i]->mVertexOrigin;
            exists = false;
            for(int j=0; j<int(sur2.size()); j++){
                if(sur2[j]->mVertexOrigin->mId == curVertex->mId){
                    exists = true;
                    break;
                }
            }
            if(!exists) {
                unconnected.push_back(curVertex);
                break;
            }
        }
        for(int i=0; i<int(sur1.size()); i++){
            if(curVertex->mId != sur1[i]->mVertexOrigin->mId){
                nextVertex = sur1[i]->mVertexDestination;
                unconnected.push_back(nextVertex);
                break;
            }
        }
    } else {
        for(int i=0; i<int(sur2.size()); i++){
            curVertex = sur2[i]->mVertexOrigin;
            exists = false;
            for(int j=0; j<int(sur1.size()); j++){
                if(sur1[j]->mVertexOrigin->mId == curVertex->mId){
                    exists = true;
                    break;
                }
            }
            if(!exists) {
                unconnected.push_back(curVertex);
                break;
            }
        }
        for(int i=0; i<int(sur2.size()); i++){
            if(curVertex->mId == sur2[i]->mVertexOrigin->mId){
                // sur2[i]->print();
                // sur2[i]->mEdgeRightCW->print();
                // sur2[i]->mEdgeRightCCW->print();
                nextVertex = sur2[i]->mEdgeRightCW->mVertexDestination;
                unconnected.push_back(nextVertex);
                break;
            }
        }
    }

    return unconnected; 
}

void mesh::Face::createDiagonal(){
    std::vector<mesh::Vertex*> surVertices = getSurroundingVertices();
    assert(surVertices.size() == 4); // only on quads

    float minDiag = INFINITY;
    mesh::Vertex* v1;
    mesh::Vertex* v2;

    for(int i=0; i<2; i++){
        float curDist = maths::Vector3::distance(surVertices[i]->mCoords, surVertices[i+2]->mCoords);
        if(curDist < minDiag){
            v1 = surVertices[i];
            v2 = surVertices[i+2];
            minDiag = curDist;
        }
    }

    mesh::Diagonal* diag = new mesh::Diagonal();
    diag->face = (mesh::Face*)this;
    diag->v1 = v1;
    diag->v2 = v2;
    diag->length = minDiag;

    mDiagonal = diag;
}

mesh::Diagonal* mesh::Face::cmpDiagonal(mesh::Diagonal* d1, mesh::Diagonal* d2){
    if(d1->length < d2->length) return d1;
    return d2;
}


std::vector<mesh::Diagonal*> mesh::Face::getMinHeap(std::vector<mesh::Face*> faces){
    // get all the diagonals
    std::vector<mesh::Diagonal*> diags;
    for(int i=0; i<int(faces.size()); i++) diags.push_back(faces[i]->mDiagonal);
    std::make_heap(diags.begin(), diags.end(), mesh::Face::cmpDiagonal);
    return diags;
}
