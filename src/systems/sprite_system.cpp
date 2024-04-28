#include "sprite_system.h"
#include "../ecs/coordinator.h"
#include "../components/sprite.h"
#include "../components/transform.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

extern Coordinator GCR;

void SpriteSystem::update(float time)
{
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, time, glm::vec3(0.0f, 1.0f, 0.0f));

    // fixed update
    for (auto const& entity : entities)
    {
        auto& sprite = GCR.get_component<Sprite>(entity);
        auto& movement = GCR.get_component<Transform>(entity);

        glm::mat4 translation = glm::mat4(1.0f);
        translation = glm::translate(translation, movement.pos);
        translation = translation * rotation;

        // render
        safety::entry_guard("Rendering a Sprite");
        sprite.texture->bind();
        sprite.shader->use();
        unsigned int loc = glGetUniformLocation(sprite.shader->ID, "transform");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(translation));
        glBindVertexArray(sprite.VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 3); // @FIXME: COUNT IS HARDCODED MY DUDE!
    }
}