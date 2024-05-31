#include "background_system.h"
#include "../components/sprite.h"
#include "../components/state.h"
#include "../components/tags/particle.h"
#include "../components/transform.h"
#include "../components/velocity.h"
#include "../ecs/coordinator.h"
#include "../sprite_cache.h"

extern Coordinator GCR;

void BackgroundSystem::init() {}

void BackgroundSystem::update(float time) {
  if (last_emit + cooldown < time) {
    Entity gstar_loc = make_star();
    float x_loc = x_distr(gen);
    auto &gstar_movement = GCR.get_component<Transform>(gstar_loc);
    gstar_movement.pos.x = x_loc;
    gstar_movement.pos.y = 9.5f;
    auto &gstar_force = GCR.get_component<Velocity>(gstar_loc).force;
    gstar_force.y = -3.0f;
    auto &gstar_active = GCR.get_component<State>(gstar_loc).active;
    gstar_active = true;
    last_emit = time;
  }
}

Entity BackgroundSystem::make_star() {
  Entity particle_star = GCR.create_entity();
  GCR.add_component(particle_star, State{
                                       .active = false,
                                   });
  float which_sprite = x_distr(gen);
  if (which_sprite < 3.33f) {
    GCR.add_component(particle_star, SpriteCache::get_sprite("bgstar1"));
  } else if (3.33f <= which_sprite && which_sprite <= 6.66f) {
    GCR.add_component(particle_star, SpriteCache::get_sprite("bgstar2"));
  } else {
    GCR.add_component(particle_star, SpriteCache::get_sprite("bgstar3"));
  }
  GCR.add_component(particle_star, Particle{});
  GCR.add_component(particle_star,
                    Transform{.pos = glm::vec3(0.0f, 0.0f, -1.0f),
                              .origin = glm::vec3(0.0f, 0.0f, -1.0f)});
  GCR.add_component(particle_star, Velocity{});
  return particle_star;
}
