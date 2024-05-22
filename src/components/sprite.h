#ifndef SPRITE_H
#define SPRITE_H

#include <glad.h>
#include "../shader.h"
#include "../texture.h"

/**
 * 
 * Not sure if pursuing a 1d float array is the play here...
 * but the Sprite vertices need to be in this format:
 * x_vert_pos, y_vert_pos, x_tex_pos, y_tex_pos
 * 
*/

struct Sprite {
    Shader *shader;
    Texture *texture;
    glm::vec3 scale_factor = glm::vec3(0.10);
    float transparency = 1.0f;
    GLuint VAO;
    GLuint VBO;
    float *vertex_data;
    uint vertex_count;

    void setup() {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertex_count * 5 * sizeof(float), vertex_data, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

};

#endif