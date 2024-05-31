#ifndef SHADER_H
#define SHADER_H
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else

#include <glad.h>

#endif

#include <GLFW/glfw3.h>
#include <fstream>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  Shader(){};
  GLuint ID;

  void compile(const char *vert_src, const char *frag_src);

  void use();
};

#endif