#pragma once

#include <lib/glm/glm.hpp>
#include <lib/glm/ext.hpp>

using namespace glm;

class Camera {
    void UpdateVectors();
public:
    vec3 front;
    vec3 up;
    vec3 right;
    mat4 rotation;

    float fov;
    vec3 position;
    Camera(vec3 position, float fov);

    void Rotate(float x, float y, float z);

    mat4 GetProjection();
    mat4 GetView();
};