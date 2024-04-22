#include "sprite_system.h"
#include "../ecs/coordinator.h"
#include "../components/sprite.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

extern Coordinator GCR;

void SpriteSystem::update(float time)
{
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::rotate(transform, time, glm::vec3(0.0f, 1.0f, 0.0f));

    // fixed update
    for (auto const& entity : entities)
    {
        auto& sprite = GCR.get_component<Sprite>(entity);
        // render
        safety::entry_guard("Hello?");
        sprite.texture->bind();
        sprite.shader->use();
        unsigned int loc = glGetUniformLocation(sprite.shader->ID, "transform");
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(sprite.VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 3); // @FIXME: COUNT IS HARDCODED MY DUDE!
        safety::entry_guard("Exit YO WAHT HAPPENED?");
    }
}