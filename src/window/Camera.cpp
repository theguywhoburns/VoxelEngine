#include "Camera.hpp"
#include "Window.hpp"

void Camera::UpdateVectors() {
    front = vec3(rotation * vec4(0.0f, 0.0f,-1.0f, 1.0f));
    right = vec3(rotation * vec4(1.0f, 0.0f, 0.0f,1.0f));
    up    = vec3(rotation * vec4(0.0f, 1.0f, 0.0f, 1.0f));
}

Camera::Camera(vec3 position, float fov) : position(position), fov(fov), rotation(1.0f) {
    UpdateVectors();
}

void Camera::Rotate(float x, float y, float z) {
    rotation = rotate(rotation, z, vec3(0.0f, 0.0f, 1.0f));
    rotation = rotate(rotation, y, vec3(0.0f, 1.0f, 0.0f));
    rotation = rotate(rotation, x, vec3(1.0f, 0.0f, 0.0f));
    UpdateVectors();
}

mat4 Camera::GetProjection() {
    float aspect = (float)Window::width / (float)Window::height;
    return perspective(fov, aspect, 0.1f, 100.0f);
}

mat4 Camera::GetView() {
    return glm::lookAt(position, position+front, up);
}
