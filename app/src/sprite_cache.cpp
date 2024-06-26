#include "sprite_cache.h"

std::unordered_map<std::string, Sprite> SpriteCache::sprites;

Sprite SpriteCache::load_sprite(Shader &shader, Texture &texture,
                                float *vertex_data, int vertex_count,
                                std::string name) {
  sprites[name] = setup_sprite(shader, texture, vertex_data, vertex_count);
  return sprites[name];
}

Sprite SpriteCache::get_sprite(const std::string &name) {
  return sprites[name];
}

Sprite SpriteCache::setup_sprite(Shader &shader, Texture &texture,
                                 float *vertex_data, int vertex_count) {
  Sprite sprite;
  sprite.shader = &shader;
  sprite.texture = &texture;
  sprite.vertex_data = vertex_data;
  sprite.vertex_count = vertex_count;
  sprite.setup();
  return sprite;
}