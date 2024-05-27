#include "text_system.h"
#include "../ecs/coordinator.h"
#include "src/components/hitbox.h"

// Credit: https://learnopengl.com/In-Practice/Text-Rendering

extern Coordinator GCR;

void TextSystem::update() {
    if (!in_game) {
        show_title();
    } else {
        show_ui();
    }
}

void TextSystem::show_title() {
    render_text("SHMUP", 10.0f, 10.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    render_text("press j to start...", 620.0f, 11.0f, 0.5f, glm::vec3(1.0f, 1.0f, 0.0f));
}

void TextSystem::show_ui() {
    int hp = GCR.get_component<Hitbox>(Entities::PLAYER).health;
    render_text("Health: " + std::to_string(hp), 210.0f, 570.0f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
    render_text("Bombs: " + std::to_string(bombs_left) + "/3", 10.0f, 570.0f, 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
    if (boss_mode) {
        int boss_hp = GCR.get_component<Hitbox>(Entities::BOSS).health;
        render_text("Boss Health: " + std::to_string(boss_hp) + "/800", 410.0f, 570.0f, 0.5f, glm::vec3(1.0f, 0.647f, 0.0f));
    } else {
        render_text("Level: " + std::to_string(checkpoint), 410.0f, 570.0f, 0.5f, glm::vec3(1.0f, 0.0f, 1.0f));
    }
}

void TextSystem::init(Shader &shader, FT_Library &ft, FT_Face &face) {
    GCR.add_listener(METHOD_LISTENER(Events::Game::BOMB_USED, TextSystem::bomb_used));
    GCR.add_listener(METHOD_LISTENER(Events::Game::BOSS_TIME, TextSystem::boss_time));
    GCR.add_listener(METHOD_LISTENER(Events::Game::WAVE_DONE, TextSystem::next_wave));
    GCR.add_listener(METHOD_LISTENER(Events::Game::START, TextSystem::start_game));
    GCR.add_listener(METHOD_LISTENER(Events::Game::STOP, TextSystem::stop_game));

    text_shader = &shader;
    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextSystem::render_text(std::string text, float x, float y, float scale, glm::vec3 color) {
    text_shader->use();
    glUniform3f(glGetUniformLocation(text_shader->ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = Characters[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;
        float vertices[6][4] = {
                {xpos,     ypos + h, 0.0f, 0.0f},
                {xpos,     ypos,     0.0f, 1.0f},
                {xpos + w, ypos,     1.0f, 1.0f},

                {xpos,     ypos + h, 0.0f, 0.0f},
                {xpos + w, ypos,     1.0f, 1.0f},
                {xpos + w, ypos + h, 1.0f, 0.0f}
        };
        glBindTexture(GL_TEXTURE_2D, ch.tex_id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextSystem::bomb_used(Event &e) {
    bombs_left -= 1;
}

void TextSystem::boss_time(Event &e) {
    boss_mode = true;
}

void TextSystem::next_wave(Event &e) {
    checkpoint += 1;
}

void TextSystem::start_game(Event &e) {
    in_game = true;
}

void TextSystem::stop_game(Event &e) {
    in_game = false;
    checkpoint = 0;
    bombs_left = 3;
}