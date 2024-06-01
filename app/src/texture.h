#ifndef TEXTURE_H
#define TEXTURE_H

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define GL_GLEXT_PROTOTYPES
#define EGL_EGLEXT_PROTOTYPES
#else

#include <glad.h>

#endif

#include <GLFW/glfw3.h>

class Texture {
public:
  unsigned int ID;
  unsigned int width, height;
  unsigned int internal_format;
  unsigned int image_format;
  unsigned int wrap_s;
  unsigned int wrap_t;
  unsigned int filter_min;
  unsigned int filter_max;

  Texture();

  void generate(unsigned int width, unsigned int height, unsigned char *data);

  void bind() const;
};

#endif
