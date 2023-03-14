#pragma once

#include "opengl.hpp"
#include "maths.hpp"

namespace opengl{

/**
 * A class to manipulate shaders
*/
class Shader{

    public:
        /**
         * The program's ID
        */
        GLuint mId;

    public:
        /**
         * A constructor taking the paths to the shaders as input
         * @param vertex The path to the vertex shader
         * @param fragment The path to the fragment shader
        */
        Shader(const std::string& vertex, const std::string& fragment);

        /**
         * Activate the shader
        */
        void use(){
            glUseProgram(mId);
        }

        /**
         * Set a given boolean value to a given uniform variable
         * @param name The uniform variable
         * @param value The new value we want to assign
        */
        void setBool(const std::string &name, bool value) const {         
            glUniform1i(glGetUniformLocation(mId, name.c_str()), (int)value); 
        }

        /**
         * Set a given integer value to a given uniform variable
         * @param name The uniform variable
         * @param value The new value we want to assign
        */
        void setInt(const std::string &name, int value) const   { 
            glUniform1i(glGetUniformLocation(mId, name.c_str()), value); 
        }
        
        /**
         * Set a given float value to a given uniform variable
         * @param name The uniform variable
         * @param value The new value we want to assign
        */
        void setFloat(const std::string &name, float value) const { 
            glUniform1f(glGetUniformLocation(mId, name.c_str()), value); 
        } 

        /**
         * Set a given matrix value to a given uniform variable
         * @param name The uniform variable
         * @param value The new value we want to assign
        */
        void setMat4fv(const std::string &name, glm::mat4 value) const { 
            glUniformMatrix4fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); 
        } 

        /**
         * Set a given vertex value to a given uniform variable
         * @param name The uniform variable
         * @param value The new value we want to assign
        */
        void setVec3f(const std::string &name, glm::vec3 value) const { 
            glUniform3f(glGetUniformLocation(mId, name.c_str()), value.x, value.y, value.z);
        } 

        /**
         * Set a given vertex value to a given uniform variable
         * @param name The uniform variable
         * @param x The new x's value we want to assign
         * @param y The new y's value we want to assign
         * @param z The new z's value we want to assign
        */
        void setVec3f(const std::string &name, float x, float y, float z) const { 
            glUniform3f(glGetUniformLocation(mId, name.c_str()), x, y, z);
        } 

};

}