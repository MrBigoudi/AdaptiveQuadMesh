#pragma once

#include <vector>
#include <string>

#include "mesh.hpp"
#include "opengl.hpp"
#include "camera/camera.hpp"

namespace scene{

enum RotationAxe{
    x,y,z
};

/**
 * A class to manipulate the objects plotted on screen
*/
class Object{

    public:
        /**
         * The list of vertices to draw
        */
        std::vector<float> mVertices;

        /**
         * The list of indices
        */
        std::vector<unsigned int> mIndices;

        /**
         * The mesh
        */
        mesh::Mesh* mMesh;

        /**
         * The vao, vbo and ebo
        */
        GLuint mVao, mVbo, mEbo;

        /**
         * The transformation matrix
        */
        glm::mat4 mTrans;

        /**
         * The dimensions of the object
        */
        float mHeight, mWidth, mDepth, mMinHeight, mMinWidth, mMinDepth, mMaxHeight, mMaxWidth, mMaxDepth;

        /**
         * Tells if the object has already been transformed
        */
        bool mIsQuad = false;

        /**
         * Init a camera's position
         * @param camera The camera to update
        */
        void initCamera(scene::Camera* camera) const;


    public:
        /**
         * A constructor taking a file path as input
         * @param obj The file to the object file we'll use to init  
        */
        Object(std::string obj);

        /**
         * A basic destructor
        */
        ~Object();

        /**
         * Transform the mesh into a quad one
        */
        void toQuadMesh();

        /**
         * Get the vertices
         * @return The object's vertices
        */
        const float* getVertices() const;

        /**
         * Get the indices
         * @return The object's indices
        */
        const unsigned int* getIndices() const;

        /**
         * Get the number of vertices
         * @return The number of vertices
        */
        unsigned int nbVertices() const;

        /**
         * Get the number of indices
         * @return The number of indices
        */
        unsigned int nbIndices() const;

        /**
         * Create an obj file from the object
         * @param file The produced file
         * @exception Invalid_Argument if the file is not correct
        */
        void toObj(std::string file);

        /**
         * Update the object
         * @param dt The delta time between frames
         * @param rotationAngle The rotation angle
         * @param rotationAxe The rotation axe
         * @param size The size
         * @param toScale True if you want to scale the image (default false)
        */
        void update(float dt, float rotationAngle, scene::RotationAxe rotationAxe, glm::vec3 size, bool toScale = false);

        /**
         * Draw the object
         * @param shaderName The shader to use
         * @param model The model matrix
         * @param view The view matrix
         * @param proj The projection matrix
        */
        void draw(std::string shaderName, glm::mat4 model, glm::mat4 view, glm::mat4 proj) const;

    private:
        /**
         * Transform the vertices and the faces of the mesh into vertices
        */
        void initVerticesAndIndices();

        /**
         * Init the vao
        */
        void initVao();

        /**
         * Init the object's dimensions
        */
        void initDim();

};

}