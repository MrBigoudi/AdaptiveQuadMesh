#include <map>

#include "object.hpp"

void scene::Object::initDim(){
    mHeight = mMesh->getHeight();
    mWidth = mMesh->getWidth();
    mDepth = mMesh->getDepth();
    mMinHeight = mMesh->getMinHeight();
    mMinWidth = mMesh->getMinWidth();
    mMinDepth = mMesh->getMinDepth();
    mMaxHeight = mMesh->getMaxHeight();
    mMaxWidth = mMesh->getMaxWidth();
    mMaxDepth = mMesh->getMaxDepth();
}

scene::Object::Object(std::string obj){
    mMesh = new mesh::Mesh(mesh::Mesh::loadOBJ(obj));
    mTrans = glm::mat4(1.0f);
    initDim();
    initVerticesAndIndices();
    initVao();
}

scene::Object::~Object(){
    glDeleteVertexArrays(1, &mVao);
    glDeleteBuffers(1, &mVbo);
    glDeleteBuffers(1, &mEbo);
}


void scene::Object::toQuadMesh(){
    if(mIsQuad) return;
    mMesh->triToQuad();
    toObj("bin/objects/tmp.obj");
    mTrans = glm::mat4(1.0f);
    initDim();
    initVerticesAndIndices();
    initVao();
    mIsQuad = true;
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

void scene::Object::initVao(){
    const float* vertices = getVertices();
    const unsigned int* indices = getIndices();

    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mVbo);
    glGenBuffers(1, &mEbo);
    // bind vao
    glBindVertexArray(mVao);
    // copy vertices in vbo
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, nbVertices()*sizeof(float), vertices, GL_STATIC_DRAW);
    // copy indices in ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbIndices()*sizeof(unsigned int), indices, GL_STATIC_DRAW);
    // set the vertex attrib pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // unbind vao
    glBindVertexArray(0);
}

void scene::Object::draw(std::string shaderName, glm::mat4 model, glm::mat4 view, glm::mat4 proj) const {
        opengl::Shader* shader = opengl::ResourceManager::getShader(shaderName);
        assert(shader);
        assert(mVao);
        assert(mVbo);
        assert(mEbo);

        shader->use();
        shader->setMat4fv("model", model);
        shader->setMat4fv("view", view);
        shader->setMat4fv("projection", proj);
        shader->setMat4fv("trans", mTrans);

        glBindVertexArray(mVao);
        // draw faces
        glDrawElements(GL_TRIANGLES, nbIndices(), GL_UNSIGNED_INT, 0);
        // draw edges
        shader->setBool("edges", true);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, nbIndices(), GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader->setBool("edges", false);

        // unbind vao
        glBindVertexArray(0);
}


void scene::Object::update([[maybe_unused]] float dt, float rotationAngle, scene::RotationAxe rotationAxe, glm::vec3 size, bool toScale){
    // rotation
    mTrans = glm::translate(mTrans, glm::vec3(0.5f * size.x * mWidth, 0.5f * size.y * mHeight, 0.5f * size.z * mDepth)); 
    switch(rotationAxe){
        case(scene::x): // x
            mTrans = glm::rotate(mTrans, glm::radians(rotationAngle), glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case(scene::y): // y 
            mTrans = glm::rotate(mTrans, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case(scene::z): //z
            mTrans = glm::rotate(mTrans, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
            break;
        default:
            break;
    }
    mTrans = glm::translate(mTrans, glm::vec3(-0.5f * size.x * mWidth, -0.5f * size.y * mHeight, -0.5f * size.z * mDepth));

    if(toScale) mTrans = glm::scale(mTrans, glm::vec3(size)); 
}

void scene::Object::initCamera(scene::Camera* camera) const {
    camera->mMovementSpeed = std::max(std::max(mWidth, mHeight), mDepth);
    camera->mPosition = maths::Vector3((mMaxWidth - mMinWidth) / 2.0f, 
                                        (mMaxHeight - mMinHeight) / 2.0f, 
                                        (mMaxHeight - mMinHeight)*2.0f);
}
