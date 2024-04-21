#ifndef SPRITE_H
#define SPRITE_H
#include "component.h"
#include "texture.h"
#include "shader.h"
#include "position.h"
#include "utils.h"
#include <glad.h>
#include <glfw3.h>

/**
 * This is getting really complicated
 * @TODO: Sprite shouldn't be responsible for loading
 *
 */

class Sprite : public Component {
    private:
        Shader shader;
        Texture texture;
        GLuint VAO_id;
        GLuint VBO_id;
    public:
        Sprite(size_t id, Shader shader, Texture texture, float* vertices);
        void fixed_update();
};

#endif