#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

#include "shaders.hpp"

std::string loadShader(const std::string &shader){
    // Open file with pointer at last position
    std::ifstream stream(shader, std::ios::ate);
    assert(stream.is_open());

    std::string full_text;

    // get position of the current pointer (last position)
    std::streampos const s = stream.tellg();

    // resize the buffer to handle the entire text
    full_text.resize( static_cast<size_t>(s) );

    // goes back to the beginning of the file and read the entire text
    stream.seekg(0,std::ios::beg);
    stream.read(&full_text[0],s);

    stream.close();
    assert(!stream.is_open());


    return full_text;
}


GLuint createProgram(const std::string& vertexShader, const std::string& fragmentShader){
    int success;
    char infoLog[512];

    // load vertex shader
    std::string tmp = loadShader(vertexShader);
    const GLchar *vertexShaderSource = tmp.c_str();

    // compile vertex shader
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderID);
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        fprintf(stderr, "Failed to compile vertex shader: %s!\n%s\n", vertexShader.c_str(), infoLog);
        exit(EXIT_FAILURE);
    }

    // load fragment shader
    tmp = loadShader(fragmentShader);
    const GLchar* fragmentShaderSource = tmp.c_str();

    // compile fragment shader
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderID);
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        fprintf(stderr, "Failed to compile fragment shader: %s!\n%s\n", fragmentShader.c_str(), infoLog);
        glDeleteShader(vertexShaderID);
        exit(EXIT_FAILURE);
    }

    // link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragmentShaderID);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        fprintf(stderr, "Failed to link shaders !\n%s\n", infoLog);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        exit(EXIT_FAILURE);
    }

    // cleaning shaders
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return shaderProgram;
}

opengl::Shader::Shader(const std::string& vertex, const std::string& fragment){
    mId = createProgram(vertex, fragment);
}