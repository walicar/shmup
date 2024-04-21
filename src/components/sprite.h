#ifndef SPRITE_H
#define SPRITE_H
#include <glad.h>
#include "../shader.h"
#include "../texture.h"
#include "../utils.h"

/**
 * 
 * Not sure if pursuing a 1d float array is the play here...
 * but the Sprite vertices need to be in this format:
 * x_vert_pos, y_vert_pos, x_tex_pos, y_tex_pos
 * 
*/

struct Sprite 
{
    Shader shader;
    Texture texture;
    GLuint VAO;
    GLuint VBO;
    Sprite(size_t id, Shader shader, Texture texture, float* vertices) 
    {
        
        safety::entry_guard("Creating Sprite");
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        safety::exit_guard("Created Sprite");
    }
};

#endif