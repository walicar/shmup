#ifndef TEXT_SYSTEM_H
#define TEXT_SYSTEM_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include "src/ecs/system.h"
#include "inc/glm/gtc/matrix_transform.hpp"
#include <map>
#include "shader.h"
#include "src/ecs/event.h"

struct Character {
    unsigned int tex_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;

};

// @TODO: it's actually more apt to call this UI Manager
class UiManager : public System {
private:
    std::map<char, Character> Characters;
    Shader *text_shader;
    GLuint VAO;
    GLuint VBO;

    void render_text(std::string text, float x, float y, float scale, glm::vec3 color);

    int bombs_left = 3;
    bool boss_mode = false;
    int checkpoint = 0;
    bool in_game = false;

    void bomb_used(Event &e);

    void boss_time(Event &e);

    void next_wave(Event &e);

    void show_ui();

    void show_title();

    void start_game(Event &e);

    void stop_game(Event &e);


public:
    void update();

    UiManager(int width, int height, Shader &shader);
};

#endif
