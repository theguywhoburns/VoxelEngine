#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

#include <lib/glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"

Shader::Shader(unsigned int id) : id(id){

}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::use() {
    glUseProgram(id);
}

void Shader::UniformMatrix(std::string name, glm::mat4 matrix) {
    GLuint TransformLoc = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(TransformLoc, 1, GL_FALSE, glm::value_ptr(matrix));

}

Shader *load_shader(std::string vertex_filepath, std::string fragment_filepath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
 
    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try {
        vShaderFile.open(vertex_filepath);
        fShaderFile.open(fragment_filepath);
        std::stringstream vShaderStream, fShaderStream;
 
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
 
        vShaderFile.close();
        fShaderFile.close();
 
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return nullptr;
    }
    const  GLchar* vShaderCode = vertexCode.c_str();
    const  GLchar* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "SHADER::VERTEX: compilation failed, \n" << infoLog << std::endl; 
        return nullptr;
    }

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "SHADER::FRAGMENT: compilation failed, \n" << infoLog << std::endl; 
        return nullptr;
    }

    // Shaader Program
    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "SHADER::PROGRAM: linking failed, \n" << infoLog << std::endl; 
        
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(id);
        return nullptr;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return new Shader(id);
}