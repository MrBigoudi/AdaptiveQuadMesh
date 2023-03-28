#include "vertex.hpp"
#include "maths.hpp"

#include <sstream>
#include <iterator>
#include <set>

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

std::vector<mesh::Vertex*> mesh::Vertex::getSurroundingVertices(int k) const{
    std::set<mesh::Vertex*> surVertices;

    // init with k = 0
    std::vector<mesh::Face*> surFaces = getSurroundingFaces();
    for(int i=0; i<int(surFaces.size()); i++){
        std::vector<mesh::Vertex*> surVerticesTmp = surFaces[i]->getSurroundingVertices();
        for(int j=0; j<int(surVerticesTmp.size()); j++){
            surVertices.insert(surVerticesTmp[j]);
        }
    }

    for(int i=0; i<k; i++){
        std::set<mesh::Vertex*> surVerticesTmp;
        for(int j=0; j<int(surVertices.size()); j++){
            std::vector<mesh::Face*> surFaces = getSurroundingFaces();
            for(int l=0; l<int(surFaces.size()); l++){
                std::vector<mesh::Vertex*> surVerticesTmpTmp = surFaces[l]->getSurroundingVertices();
                for(int m=0; m<int(surVerticesTmpTmp.size()); m++){
                    surVerticesTmp.insert(surVerticesTmpTmp[m]);
                }
            }
        }
        surVertices.merge(surVerticesTmp);
    }

    std::vector<mesh::Vertex*> finalVertices (surVertices.begin(), surVertices.end());
    return finalVertices;
}

float mesh::Vertex::getDistance(mesh::Vertex* v2) const{
    return maths::Vector3::distance(mCoords, v2->mCoords);
}

std::vector<mesh::Vertex*> mesh::Vertex::getVerticesInRadius(std::vector<mesh::Vertex*> vertices, float r) const{
    std::vector<mesh::Vertex*> res;
    for(int i=0; i<int(vertices.size()); i++){
        mesh::Vertex* curVertex = vertices[i];
        if(getDistance(curVertex) <= r) res.push_back(curVertex);
    }
    return res;
}

float mesh::Vertex::getAverageEdgeLength(std::vector<mesh::Vertex*> vertices){
    // get the surrounging edges
    std::vector<mesh::Edge*> surEdges;
    for(int i=0; i<int(vertices.size()); i++){
        std::vector<mesh::Edge*> surEdgesTmp = vertices[i]->getSurroundingEdges();
        surEdges.insert(surEdges.end(), surEdgesTmp.begin(), surEdgesTmp.end());
    }

    // calculate the average length
    float sum = 0.0f;
    for(int i=0; i<int(surEdges.size()); i++){
        sum += surEdges[i]->getLength();
    }

    return sum / float(surEdges.size());
}

maths::Vector3* mesh::Vertex::getInterpolatedPlaneNormal(std::vector<mesh::Vertex*> vertices){
    // create the plane (Ordinary Least Squares)
    float sumX = 0.0f; float sumY = 0.0f; float sumZ = 0.0f;
    float sumXY = 0.0f; float sumXZ = 0.0f; float sumYZ = 0.0f;
    float sumXX = 0.0f; float sumYY = 0.0f;

    for(int i=0; i<int(vertices.size()); i++){
        maths::Vector3* curPoint = vertices[i]->mCoords;
        float x = curPoint->x(); float y = curPoint->y(); float z = curPoint->z();
        sumX += x; sumY += y; sumZ += z;
        sumXX += x*x; sumYY += y*y;
        sumXY += x*z; sumYZ += y*z;
    }
    glm::mat3 matA = {
        sumXX, sumXY, sumX,
        sumXY, sumYY, sumY,
        sumX, sumY, vertices.size()
    };
    glm::vec3 vecB = {sumXZ, sumYZ, sumZ};

    glm::vec3 planeCoeffs = glm::inverse(matA)*vecB;

    // convert to vector3
    return new maths::Vector3(planeCoeffs.x, planeCoeffs.y, planeCoeffs.z);
}

std::vector<float> mesh::Vertex::getSquaredDotProducts(maths::Vector3 normal, std::vector<mesh::Vertex*> vertices){
    std::vector<float> dotProds;

    for(int i=0; i<int(vertices.size()); i++){
        float curDot = maths::Vector3::dot(normal.normalize(), vertices[i]->mCoords->normalize());
        dotProds.push_back(curDot*curDot);
    }

    return dotProds;
}
