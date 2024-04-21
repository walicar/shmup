#include "sprite.h"

Sprite::Sprite(size_t id, Shader shader, Texture texture, float* vertices) :
    Component(id), shader(shader), texture(texture) {
        // not sure if pursuing a 1d float array is the play here...
        // but the Sprite vertices need to be in this format
        // x_vert_pos, y_vert_pos, x_tex_pos, y_tex_pos
        safety::entry_guard("Creating Sprite");
        glGenVertexArrays(1, &VAO_id);
        glGenBuffers(1, &VBO_id);

        glBindVertexArray(VAO_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        safety::exit_guard("Created Sprite");
}

void Sprite::fixed_update() {
    glm::mat4 transform = glm::mat4(1.0f); // identity
    transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    Position& pos = ComponentSet<Position>::get(id);
    texture.bind();
    shader.use();
    unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO_id);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 3); // @FIXME: COUNT IS HARDCODED MY DUDE!
}