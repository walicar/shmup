#ifndef SHADER_H
#define SHADER_H

#include <glad.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    Shader() {};
    unsigned int ID;

    void compile(const char *vert_src, const char *frag_src);

    void use();
};

#endif