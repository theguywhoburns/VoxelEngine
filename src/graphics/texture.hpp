#pragma once

class Texture {
public:
    unsigned int id;
    int width;
    int height;
    Texture(unsigned int id, int width, int height);
    ~Texture();

    void bind();
};