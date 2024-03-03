#pragma once

#include <lib/glm/glm.hpp>
#include <string>

class Shader {
    unsigned int id;
public:
    Shader(unsigned int id);
    ~Shader();

    void use();
    void UniformMatrix(std::string name, glm::mat4 matrix);
};

Shader* load_shader(std::string vertex_filepath, std::string fragment_filepath);