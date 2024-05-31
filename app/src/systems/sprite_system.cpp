#include "sprite_system.h"
#include "../components/sprite.h"
#include "../components/state.h"
#include "../components/tags/player.h"
#include "../components/transform.h"
#include "../ecs/coordinator.h"
#include <gtc/type_ptr.hpp>

extern Coordinator GCR;

void SpriteSystem::update(float time) {
  glm::mat4 rotation = glm::mat4(1.0f);
  rotation = glm::rotate(rotation, time, glm::vec3(0.0f, 1.0f, 0.0f));

  // fixed update
  for (auto const &entity : entities) {
    auto &state = GCR.get_component<State>(entity);
    if (!state.active)
      continue;

    auto &movement = GCR.get_component<Transform>(entity);

    if (entity >= Entities::G_STAR) {
      movement.pos.z = 9.5f;
    } else {
      movement.pos.z = 1.5f;
    }

    auto &sprite = GCR.get_component<Sprite>(entity);
    auto transform = glm::mat4(1.0f);
    transform = glm::scale(transform, sprite.scale_factor);
    transform = glm::translate(transform, movement.pos);

    if (entity == Entities::PLAYER &&
        GCR.get_component<Player>(Entities::PLAYER).rotate) {
      transform = transform * rotation;
    }

    bool isOffscreen = (movement.pos.x < -10.0f || movement.pos.x > 10.0f ||
                        movement.pos.y < -10.0f || movement.pos.y > 10.0f);

    // render
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    sprite.texture->bind();
    sprite.shader->use();
    unsigned int loc = glGetUniformLocation(sprite.shader->ID, "transform");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(transform));
    unsigned int tloc = glGetUniformLocation(sprite.shader->ID, "transparency");
    glUniform1f(tloc, sprite.transparency);
    glBindVertexArray(sprite.VAO);

    if (entity == Entities::PLAYER ||
        (entity >= Entities::E_GRUNT && entity <= Entities::BOSS)) {
      glDrawArrays(GL_TRIANGLES, 0, 3);
    } else {
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }

    // deactivate particles that are OOB
    if (isOffscreen && entity != Entities::PLAYER) {
      state.active = false;
    }
    if (entity >= Entities::G_STAR && isOffscreen) {
      trash.push_back(entity);
    }
  }

  // delete the star particles that are dynamically generated
  if (!trash.empty()) {
    for (Entity entity : trash)
      GCR.destroy_entity(entity);
    trash.clear();
  }
}