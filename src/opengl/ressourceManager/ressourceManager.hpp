/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#pragma once

#include <map>
#include <string>

#include "shaders/shaders.hpp"

namespace opengl{

class Shader;

/**
 * A static singleton ResourceManager class that hosts several
 * functions to load Textures and Shaders. Each loaded texture
 * and/or shader is also stored for future reference by string
 * handles. All functions and resources are static and no 
 * public constructor is defined.
*/
class ResourceManager {

    public:
        /*
         * resource storage
        */
        static std::map<std::string, opengl::Shader*> mShaders;

    public:
        /**
         * loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
         * @param vsFile The path to the vertex shader
         * @param fsFile The path to the fragment shader
         * @param name The shader's name
         * @return The newly created shader
        */
        static Shader* loadShader(const std::string & vsFile, const std::string & fsFile, const std::string & name);
        
        /**
         * retrieves a stored sader
         * @param name The shader's name
         * @return The corresponding shader
        */
        static opengl::Shader* getShader(std::string name);

        /**
         * properly de-allocates all loaded resources
        */
        static void clear();

    private:
        /**
         * private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).        
        */
        ResourceManager() {};


};

};