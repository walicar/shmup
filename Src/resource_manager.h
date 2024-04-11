#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <string>
#include <glad.h>
#include "shader.h"

class ResourceManager {
    public:
        static std::unordered_map<std::string, Shader> shaders;
        
        static Shader load_shader(const char *vert_src, const char *frag_src, std::string name);
        static Shader get_shader(std::string name);
        
        static void close();
    private:
        ResourceManager() {};
        static Shader load_shader_file(const char *vert_src, const char *frag_src);
};

#endif