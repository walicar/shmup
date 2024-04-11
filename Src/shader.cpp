#include "shader.h"

Shader::Shader(const char *vert_src, const char *frag_src) {
    std::string vert_code;
    std::string frag_code;
    std::ifstream vert_file;
    std::ifstream frag_file;

    vert_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    frag_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vert_file.open(vert_src);
        frag_file.open(frag_src);
        std::stringstream vert_stream, frag_stream;
        vert_stream << vert_file.rdbuf();
        frag_stream << frag_file.rdbuf();
        vert_file.close();
        frag_file.close();
        vert_code = vert_stream.str();
        frag_code = frag_stream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }
    unsigned int vert_id, frag_id;
    int success;
    char info_log[512];

    const char* vert_ptr = vert_code.c_str();
    vert_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_id, 1, &vert_ptr, NULL);
    glCompileShader(vert_id);
    glGetShaderiv(vert_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vert_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    };

    const char* frag_ptr = frag_code.c_str();
    frag_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_id, 1, &frag_ptr, NULL);
    glCompileShader(frag_id);
    glGetShaderiv(frag_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(frag_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    };

    ID = glCreateProgram();
    glAttachShader(ID, vert_id);
    glAttachShader(ID, frag_id);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }

    glDeleteShader(vert_id);
    glDeleteShader(frag_id);   
};

void Shader::use() {
    glUseProgram(ID);
}
