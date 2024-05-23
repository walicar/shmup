#ifndef SPRITE_CACHE_H
#define SPRITE_CACHE_H

#include <unordered_map>
#include "components/sprite.h"

class SpriteCache {
public:
    static std::unordered_map<std::string, Sprite> sprites;

    static void load_sprite(Shader &shader, Texture &texture, float* vertex_data, int vertex_count, std::string name);

    static Sprite get_sprite(std::string name);

    static void close();

private:
    SpriteCache() {};

    static Sprite setup_sprite(Shader &shader, Texture &texture, float* vertex_data, int vertex_count);
};

#endif
