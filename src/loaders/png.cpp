#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <lib/stb/stb_image.h>
#include <graphics/texture.hpp>

#include "png.hpp"

int _png_load(const char* file, int* width, int* height){
    // Load the image
    int _width, _height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip the image's y-axis
    unsigned char* data = stbi_load(file, &_width, &_height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture" << std::endl;
        return 0;
    }

    // Update the width and height in the caller's scope
    if (width) *width = _width;
    if (height) *height = _height;

    // Determine the image format
    GLenum format = GL_RGB;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0,
    format, GL_UNSIGNED_BYTE, (GLvoid *) data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

Texture *load_texture(std::string filename) {
    int width, height;
    GLuint texture = _png_load(filename.c_str(), &width, &height);
    if(texture == 0) {
        std::cerr << "Could not create texture " << filename << std::endl;
        return nullptr;
    }

    return new Texture(texture, width, height);
}
