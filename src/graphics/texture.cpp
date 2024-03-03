#include <GL/glew.h>
#include "texture.hpp"

Texture::Texture(unsigned int id, int width, int height) : id(id), width(width), height(height) {

}

Texture::~Texture() {
    glDeleteTextures(1, &id);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}
