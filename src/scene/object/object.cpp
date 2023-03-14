#include <map>

#include "object.hpp"

scene::Object::Object(std::string obj){
    mMesh = new mesh::Mesh(mesh::Mesh::loadOBJ(obj));
    initVerticesAndIndices();
}


void scene::Object::toQuadMesh(){
    mMesh->triToQuad();
    initVerticesAndIndices();
}


const float* scene::Object::getVertices() const { return mVertices.data(); }


const unsigned int* scene::Object::getIndices() const { return mIndices.data(); }


unsigned int scene::Object::nbVertices() const { return mVertices.size(); }


unsigned int scene::Object::nbIndices() const { return mIndices.size(); }


void scene::Object::initVerticesAndIndices(){
    // init a hash map to transform the mesh vertices' indices into values from 0 to nbVertices 
    std::map<int, int> verticesIdx;
    for(int i=0; i<mMesh->mNbVertices; i++){
	    verticesIdx[mMesh->mVertices[i]->mId] = i;
        // init the vertices
        glm::vec3 curVec = mMesh->mVertices[mMesh->mNbVertices-i-1]->toGlm();
        mVertices.push_back(curVec.x);
        mVertices.push_back(curVec.y);
        mVertices.push_back(curVec.z);
    }

    // init the indices
    for(int i=0; i<mMesh->mNbFaces; i++){
        std::vector<mesh::Vertex*> verticesTmp = mMesh->mFaces[i]->getSurroundingVertices();
        for(int j=0; j<int(verticesTmp.size()); j++){
            mIndices.push_back(verticesIdx[verticesTmp[j]->mId]);
        }
    }

}

void scene::Object::toObj(std::string file){
    mMesh->toObj(file);
}