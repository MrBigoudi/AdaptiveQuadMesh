/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/

#include "ressourceManager.hpp"

// instantiate static variables
std::map<std::string, opengl::Shader*> opengl::ResourceManager::mShaders;

opengl::Shader* opengl::ResourceManager::loadShader(const std::string & vsFile, const std::string & fsFile, const std::string & name){
    mShaders[name] = new opengl::Shader(vsFile, fsFile);
    return mShaders[name];
}

opengl::Shader* opengl::ResourceManager::getShader(std::string name){
    return mShaders[name];
}

void opengl::ResourceManager::clear(){
    // (properly) delete all shaders	
    for (auto iter : mShaders)
        glDeleteProgram(iter.second->mId);
}