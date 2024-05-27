#include "resource_manager.h"

std::unordered_map<std::string, Shader> ResourceManager::shaders;
std::unordered_map<std::string, Texture> ResourceManager::textures;

Shader ResourceManager::load_shader(const char *vert_src, const char *frag_src, const std::string& name) {
    shaders[name] = load_shader_file(vert_src, frag_src);
    return shaders[name];
}

Shader ResourceManager::get_shader(const std::string& name) {
    return shaders[name];
}

Texture ResourceManager::load_texture(const char *file, const std::string& name, bool alpha) {
    textures[name] = load_texture_file(file, alpha);
    return textures[name];
}

Texture ResourceManager::get_texture(const std::string& name) {
    return textures[name];
}

void ResourceManager::close() {
    for (const auto& iter: shaders)
        glDeleteProgram(iter.second.ID);
}

Shader ResourceManager::load_shader_file(const char *vert_src, const char *frag_src) {
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

    Shader shader;
    shader.compile(vert_code.c_str(), frag_code.c_str());
    return shader;
};

Texture ResourceManager::load_texture_file(const char *file, bool alpha) {
    Texture texture;
    if (alpha) {
        texture.internal_format = GL_RGBA;
        texture.image_format = GL_RGBA;
    }

    int width, height, channel_num;
    // weird bug: https://computergraphics.stackexchange.com/questions/9671/texture-loading-erratic-working
    unsigned char *data = stbi_load(file, &width, &height, &channel_num, 4);
    channel_num = 4;
    texture.generate(width, height, data);
    stbi_image_free(data);
    return texture;
}