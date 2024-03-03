#pragma once

class Mesh;
class Chunk;

class VoxelRenderer {
    float* buffer;
    unsigned long long capacity;
public:
    VoxelRenderer(unsigned long long capacity);
    ~VoxelRenderer();

    Mesh* Render(Chunk* chunk, const Chunk** chunks);
};