#include "shader.h"

void Shader::compile(const char *vert_code, const char *frag_code) {
  GLuint vert_id, frag_id;
  int success;
  char info_log[512];

  vert_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_id, 1, &vert_code, nullptr);
  glCompileShader(vert_id);
  glGetShaderiv(vert_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vert_id, 512, nullptr, info_log);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << info_log << std::endl;
  };

  frag_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_id, 1, &frag_code, nullptr);
  glCompileShader(frag_id);
  glGetShaderiv(frag_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(frag_id, 512, nullptr, info_log);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << info_log << std::endl;
  };

  ID = glCreateProgram();
  glAttachShader(ID, vert_id);
  glAttachShader(ID, frag_id);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, nullptr, info_log);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << info_log << std::endl;
  }

  glDeleteShader(vert_id);
  glDeleteShader(frag_id);
}

void Shader::use() { glUseProgram(ID); }
