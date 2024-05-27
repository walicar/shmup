#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <string>
#include <glad.h>
#include "shader.h"
#include "texture.h"
#include "stb_image.h"

class ResourceManager {
public:
    static std::unordered_map<std::string, Shader> shaders;
    static std::unordered_map<std::string, Texture> textures;

    static Shader load_shader(const char *vert_src, const char *frag_src, const std::string& name);

    static Shader get_shader(const std::string& name);

    static Texture load_texture(const char *file, const std::string& name, bool alpha);

    static Texture get_texture(const std::string& name);

    static void close();

private:
    ResourceManager() {};

    static Shader load_shader_file(const char *vert_src, const char *frag_src);

    static Texture load_texture_file(const char *file, bool alpha);
};

#endif