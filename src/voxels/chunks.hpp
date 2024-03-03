#pragma once


#include <stdlib.h>
#include <lib/glm/glm.hpp>

using namespace glm;

class Chunk;
class voxel;

class Chunks {
public:
	Chunk** chunks;
	size_t volume;
	unsigned int w,h,d;

	Chunks(int w, int h, int d);
	~Chunks();

	Chunk* GetChunk(int x, int y, int z);
	voxel* Get(int x, int y, int z);
	void Set(int x, int y, int z, int id);
	voxel* RayCast(vec3 start, vec3 dir, float maxLength, vec3& end, vec3& norm, vec3& iend);
};
