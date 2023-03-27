#include "vertex.hpp"

#include <sstream>
#include <iterator>

int mesh::Vertex::ID_CPT = 0;

std::string mesh::Vertex::toString() const {
    char buffer[50];
    sprintf(buffer, "v%d -> e%d, toDel = %d", mId, mEdge->mId, mToDelete);
    return buffer;
}

void mesh::Vertex::print() const {
    fprintf(stdout, "%s\n", toString().c_str());
}


std::vector<mesh::Vertex*> mesh::Vertex::getCommonVertices(mesh::Face* f1, mesh::Face* f2){
    std::vector<mesh::Vertex*> surV1 = f1->getSurroundingVertices();
    std::vector<mesh::Vertex*> surV2 = f2->getSurroundingVertices();

    std::vector<mesh::Vertex*> common;

    std::vector<mesh::Vertex*> tmp = surV1;
    if(int(surV1.size()) < int((surV2.size()))){
        surV1 = surV2;
        surV2 = tmp;
    }

    for(int i=0; i<int(surV1.size()); i++){
        int v1Id = surV1[i]->mId;
        for(int j=0; j<int(surV2.size()); j++){
            int v2Id = surV2[j]->mId;

            if(v1Id == v2Id){
                common.push_back(surV1[i]);
            }
        }
    }
    return common;
}



mesh::Vertex* mesh::Vertex::getCommonVertex(mesh::Face* f1, mesh::Face* f2, mesh::Face* f3){
    std::vector<mesh::Vertex*> commonf1f2 = mesh::Vertex::getCommonVertices(f1, f2);
    int size = commonf1f2.size();
    if( size == 0) return nullptr;

    std::vector<mesh::Vertex*> surV3 = f3->getSurroundingVertices();
    for(int i=0; i<int(surV3.size()); i++){
        for(int j=0; j<size; j++){
            if(surV3[i]->mId == commonf1f2[j]->mId) return surV3[i];
        }
    }

    return nullptr;
}

bool mesh::Vertex::isInList(std::vector<mesh::Vertex*> list) const{
    for(int i=0; i<int(list.size()); i++){
        if(list[i]->mId == mId) return true;
    }
    return false;
}


mesh::Vertex* mesh::Vertex::getIsolatedVertex(mesh::Face* faceToIsolate, mesh::Face* f2, mesh::Face* f3){
    std::vector<mesh::Vertex*> surV1 = faceToIsolate->getSurroundingVertices();

    std::vector<mesh::Vertex*> surV2 = f2->getSurroundingVertices();
    std::vector<mesh::Vertex*> surV3 = f3->getSurroundingVertices();

    for(int i=0; i<int(surV1.size()); i++){
        mesh::Vertex* curVertex = surV1[i];
        // if the vertex is isolated
        if(!curVertex->isInList(surV2) && !curVertex->isInList(surV3))
            return curVertex;
    }

    return nullptr;
}


mesh::Vertex* mesh::Vertex::getOppositeVertex(mesh::Face* quad, mesh::Vertex* v){
    if(quad->isTriangle()) return nullptr;
    mesh::Edge* edge = quad->mEdge;
    while(edge->mVertexOrigin->mId != v->mId) edge = edge->mEdgeRightCW;
    return edge->mEdgeRightCW->mVertexDestination;
}




std::string mesh::Vertex::listToString(std::vector<mesh::Vertex*> list){
    std::vector<std::string> strings;
    for(int i=0; i<int(list.size()); i++){
        char buffer[50];
        sprintf(buffer, "v%d", list[i]->mId);
        strings.push_back(buffer);
    }

    const char* const delim = ", ";

    std::ostringstream imploded;
    std::copy(strings.begin(), strings.end(),
            std::ostream_iterator<std::string>(imploded, delim));
    return imploded.str();
}

bool mesh::Vertex::twoSameEdges(std::vector<mesh::Edge*> edges, int nbVertices){
	std::vector<std::vector<mesh::Edge*>> table(nbVertices, std::vector<mesh::Edge*>(nbVertices, nullptr));
    // printf("\nBefore nbVertices: %d\n", nbVertices);
    for(int i=0; i<int(edges.size()); i++){
        int v0 = edges[i]->mVertexOrigin->mId;
        int v1 = edges[i]->mVertexDestination->mId;
        // edges[i]->print();
        // printf("nbVertices: %d\n", nbVertices);
        if(table[v0][v1] != nullptr) {
            // table[v0][v1]->print();
            return true;
        }
        table[v0][v1] = edges[i];
    }
    return false;
}

glm::vec3 mesh::Vertex::toGlm() const{
    return mCoords->toGlm();
}

std::vector<mesh::Face*> mesh::Vertex::getSurroundingFaces() const{
    std::vector<mesh::Face*> faces;

    mesh::Edge* e0;
    mesh::Edge* curEdge;
    
    if (mEdge->mVertexOrigin->mId == mId){
        e0 = mEdge;
        curEdge = mEdge;
    } else {
        e0 = mEdge->mReverseEdge;
        curEdge = mEdge->mReverseEdge;
        assert(false);
    }
    
    do{
        // curEdge->print(); e0->print();
        assert(curEdge->mVertexOrigin->mId == mId);
        curEdge = curEdge->mEdgeRightCCW->mReverseEdge;
        assert(curEdge);
        assert(curEdge->mFaceRight);
        faces.push_back(curEdge->mFaceRight);
    } while(curEdge->mId != e0->mId);
    
    return faces;
}

std::vector<mesh::Edge*> mesh::Vertex::getSurroundingEdges() const{
    std::vector<mesh::Edge*> edges;

    mesh::Edge* e0;
    mesh::Edge* curEdge;
    
    if (mEdge->mVertexOrigin->mId == mId){
        e0 = mEdge;
        curEdge = mEdge;
    } else {
        e0 = mEdge->mReverseEdge;
        curEdge = mEdge->mReverseEdge;
    }

    do{
        assert(curEdge->mVertexOrigin->mId == mId);
        curEdge = curEdge->mEdgeRightCCW->mReverseEdge;
        assert(curEdge);
        assert(curEdge->mReverseEdge);
        edges.push_back(curEdge);
        edges.push_back(curEdge->mReverseEdge);
    } while(curEdge->mId != e0->mId);
    
    return edges;
}

void mesh::Vertex::mergeVertices(mesh::Vertex* v2, std::vector<mesh::Edge*> edges){
    // printf("Vert to remove:\n"); v2->print();
    // printf("Vert to keep:\n"); print();
    assert(v2->mToDelete);
    for(int i=0; i<int(edges.size()); i++){
        // printf("Cur edge:\n"); surEdges[i]->print();
        if(edges[i]->mVertexOrigin->mId == v2->mId) edges[i]->mVertexOrigin = this;
        else if(edges[i]->mVertexDestination->mId == v2->mId) edges[i]->mVertexDestination = this;
        else assert(false);
    }
}