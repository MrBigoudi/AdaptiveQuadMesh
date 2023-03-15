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
    // toQuadMesh();
    initVerticesAndIndices();
    initDim();
    initVao();
}

void scene::Object::cleanVao(){
    glDeleteVertexArrays(1, &mVao);
    glDeleteVertexArrays(1, &mVaoLines);
    glDeleteBuffers(1, &mVbo);
    glDeleteBuffers(1, &mEbo);
    glDeleteBuffers(1, &mEboLines);    
}

void scene::Object::cleanLists(){
    mVertices.clear();
    mIndices.clear();
    mLinesIndices.clear();
}


scene::Object::~Object(){
    cleanVao();
}


void scene::Object::toQuadMesh(){
    if(mIsQuad) return;
    printf("\n\n###################### Begin Triangular Mesh to Quad Mesh transformation ######################\n\n");
    mMesh->triToQuad();
    toObj("bin/objects/tmp.obj");
    mTrans = glm::mat4(1.0f);
    initVerticesAndIndices();
    initDim();
    initVao();
    mIsQuad = true;
    printf("\n\n###################### Done Triangular Mesh to Quad Mesh transformation ###################\n\n");
}


const float* scene::Object::getVertices() const { return mVertices.data(); }

const unsigned int* scene::Object::getIndices() const { return mIndices.data(); }

const unsigned int* scene::Object::getLinesIndices() const { return mLinesIndices.data(); }

unsigned int scene::Object::nbVertices() const { return mVertices.size(); }

unsigned int scene::Object::nbIndices() const { return mIndices.size(); }

unsigned int scene::Object::nbLinesIndices() const { return mLinesIndices.size(); }

void scene::Object::initVerticesAndIndices(){
    cleanLists();
    // init a hash map to transform the mesh vertices' indices into values from 0 to nbVertices 
    std::map<int, int> verticesIdx;
    for(int i=0; i<mMesh->mNbVertices; i++){
	    verticesIdx[mMesh->mVertices[i]->mId] = i;
        // init the vertices
        glm::vec3 curVec = mMesh->mVertices[i]->toGlm();
        mVertices.push_back(curVec.x);
        mVertices.push_back(curVec.y);
        mVertices.push_back(curVec.z);
    }

    // init the indices
    for(int i=0; i<mMesh->mNbFaces; i++){
        std::vector<mesh::Vertex*> verticesTmp = mMesh->mFaces[i]->getSurroundingVertices();
        // for quads
        if(verticesTmp.size() == 4){
            // for faces
            mIndices.push_back(verticesIdx[verticesTmp[0]->mId]);
            mIndices.push_back(verticesIdx[verticesTmp[1]->mId]);
            mIndices.push_back(verticesIdx[verticesTmp[2]->mId]);
            mIndices.push_back(verticesIdx[verticesTmp[0]->mId]);
            mIndices.push_back(verticesIdx[verticesTmp[2]->mId]);
            mIndices.push_back(verticesIdx[verticesTmp[3]->mId]);
            // for lines
            for(int j=0; j<4; j++){
                int prev = verticesIdx[verticesTmp[j]->mId];
                int next = verticesIdx[verticesTmp[(j+1) % 4]->mId];
                // first triangle
                mLinesIndices.push_back(prev);
                mLinesIndices.push_back(next);
            }
        // for triangles
        } else {
            for(int j=0; j<int(verticesTmp.size()); j++){
                mIndices.push_back(verticesIdx[verticesTmp[j]->mId]);
                mLinesIndices.push_back(verticesIdx[verticesTmp[j]->mId]);
                mLinesIndices.push_back(verticesIdx[verticesTmp[(j+1)%3]->mId]);
            }
        }
    }

}

void scene::Object::toObj(std::string file){
    mMesh->toObj(file);
}

void scene::Object::initVao(){
    const float* vertices = getVertices();
    const unsigned int* indices = getIndices();
    const unsigned int* indicesLines = getLinesIndices();

    cleanVao();

    glGenVertexArrays(1, &mVao);
    glGenVertexArrays(1, &mVaoLines);
    glGenBuffers(1, &mVbo);
    glGenBuffers(1, &mEbo);
    glGenBuffers(1, &mEboLines);

    // bind vao for faces
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

    // bind vao for lines
    glBindVertexArray(mVaoLines);
    // copy vertices in vbo
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glBufferData(GL_ARRAY_BUFFER, nbVertices()*sizeof(float), vertices, GL_STATIC_DRAW);
    // copy indices in ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEboLines);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nbLinesIndices()*sizeof(unsigned int), indicesLines, GL_STATIC_DRAW);
    // set the vertex attrib pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind vao
    glBindVertexArray(0);
}

void scene::Object::draw(std::string shaderName, glm::mat4 model, glm::mat4 view, glm::mat4 proj, float linesWidth) const {
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

        // draw lines
        glBindVertexArray(mVaoLines);
        shader->setBool("edges", true);
        assert(linesWidth >= 0.0f);
        glLineWidth(linesWidth);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_LINES, nbLinesIndices(), GL_UNSIGNED_INT, 0);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader->setBool("edges", false);
        glLineWidth(1.0f);

        // unbind vao
        glBindVertexArray(0);
}


void scene::Object::update([[maybe_unused]] float dt, float rotationAngle, scene::RotationAxe rotationAxe, glm::vec3 size, bool toScale){
    // rotation
    mTrans = glm::translate(mTrans, glm::vec3(0.5f * size.x * (mMaxWidth + mMinWidth), 0.5f * size.y * (mMaxHeight + mMinHeight), 0.5f * size.z * (mMaxDepth + mMinDepth))); 
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
    mTrans = glm::translate(mTrans, glm::vec3(-0.5f * size.x * (mMaxWidth + mMinWidth), -0.5f * size.y * (mMaxHeight + mMinHeight), -0.5f * size.z * (mMaxDepth + mMinDepth))); 

    if(toScale) mTrans = glm::scale(mTrans, glm::vec3(size)); 
}

void scene::Object::initCamera(scene::Camera* camera) const {
    // printf("mHeight: %f, mWidth: %f, mDepth: %f\nmMinWidth: %f, mMaxWidth: %f\nmMinHeight: %f, mMaxHeight:%f\nmMinDepth: %f, mMaxDepth: %f\n",
                // mHeight, mWidth, mDepth, mMinWidth, mMaxWidth, mMinHeight, mMaxHeight, mMinDepth, mMaxDepth);
    camera->mMovementSpeed = std::max(std::max(mWidth, mHeight), mDepth);
    camera->mPosition = maths::Vector3((mMaxWidth + mMinWidth) / 2.0f, 
                                        (mMaxHeight + mMinHeight) / 2.0f, 
                                        2.0f*mDepth);
    camera->mZmax = 500.0f;
    // printf("camera: %s\n", camera->mPosition.toString().c_str());
}
