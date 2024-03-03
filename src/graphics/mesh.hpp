#pragma once

class Mesh {
    unsigned int vao;
    unsigned int vbo;
    unsigned long long vertices;
public:
    Mesh(const float* buffer, unsigned long long vertices, const int* attrs);
    ~Mesh();

    void Draw(unsigned int primitive);
};