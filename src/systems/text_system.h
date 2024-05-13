#ifndef TEXT_SYSTEM_H
#define TEXT_SYSTEM_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "../ecs/system.h"
#include <gtc/matrix_transform.hpp>
#include <map>
#include "../shader.h"
#include "../ecs/event.h"

struct Character {
    unsigned int tex_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;

};

class TextSystem : public System {
private:
    std::map<char, Character> Characters;
    Shader *text_shader;
    GLuint VAO;
    GLuint VBO;

    void render_text(std::string text, float x, float y, float scale, glm::vec3 color);

    int bombs_left = 3;

    void bomb_used(Event &e);

public:
    void update();

    void init(Shader &shader, FT_Library &ft, FT_Face &face);
};

#endif
