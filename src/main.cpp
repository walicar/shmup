#include <glad.h>
#include <glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include "shader.h"
#include "resource_manager.h"
#include "sprite_cache.h"
#include "window_manager.h"
#include "ecs/coordinator.h"
#include "ecs/types.h"
#include "components/sprite.h"
#include "components/hitbox.h"
#include "systems/text_system.h"
#include "game.h"
#include <map>

Coordinator GCR;
static bool quit = false;

void handle_quit(Event &e) {
    quit = true;
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float v[] = {
        -0.25f, -0.5f, 0.0f, 0.0f, 0.0f,       // bottom left
        0.25f, -0.5f, 0.0f, 1.0f, 0.0f,       // bottom right
        0.0f, 0.5f, 0.0f, 0.5f, 1.0f        // upper middle
};

float lv[] = {
        -0.25f, -1.0f, 0.0f, 0.0f, 0.0f,
        0.25f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.25f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.25f, 1.0f, 0.0f, 0.0f, 1.0f
};

float bv[] = {
        -0.25f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.25f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.25f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.25f, 0.5f, 0.0f, 0.0f, 1.0f
};

float ev[] = {
        -0.25f, 0.5f, 0.0f, 0.0, 0.0,
        0.25f, 0.5f, 0.0f, 1.0, 0.0,
        0.0f, -0.5f, 0.0f, 0.5, 1.0
};

int main() {
    // application entry
    WindowManager window_manager("SHMUP", SCR_WIDTH, SCR_HEIGHT);
    GCR.add_listener(FUNCTION_LISTENER(Events::Window::QUIT, handle_quit)); // lambda preferred?

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "fonts/font.ttf", 0, &face)) {
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    Shader text_shader = ResourceManager::load_shader("shaders/font.vert", "shaders/font.frag", "text");
    Shader def_shader = ResourceManager::load_shader("shaders/default.vert", "shaders/default.frag", "default");
    // Texture def_texture = ResourceManager::load_texture("textures/smile.png", "smile", true);
    Texture laser_texture = ResourceManager::load_texture("textures/laser.png", "laser", true);
    Texture bomb_texture = ResourceManager::load_texture("textures/bomb.png", "bomb", true);
    Texture bullet_texture = ResourceManager::load_texture("textures/bullet.png", "bullet", true);
    Texture ship_texture = ResourceManager::load_texture("textures/ship.png", "ship", true);
    Texture ebullet_texture = ResourceManager::load_texture("textures/ebullet.png", "ebullet", true);
    Texture eship_texture = ResourceManager::load_texture("textures/eship.png", "eship", true);
    Texture snipe_texture = ResourceManager::load_texture("textures/snipe.png", "snipe", true);
    Texture star_texture = ResourceManager::load_texture("textures/star.png", "star", true);
    Texture hose_texture = ResourceManager::load_texture("textures/hose.png", "hose", true);
    Texture boss_texture = ResourceManager::load_texture("textures/boss.png", "boss", true);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    text_shader.use();
    glUniformMatrix4fv(glGetUniformLocation(text_shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    Sprite player_sprite = SpriteCache::load_sprite(def_shader, ship_texture, v, 3, "player");
    Sprite player_laser_sprite = SpriteCache::load_sprite(def_shader, laser_texture, lv, 4, "plaser");
    Sprite player_bomb_sprite = SpriteCache::load_sprite(def_shader, bomb_texture, bv, 4, "pbomb");
    Sprite player_bullet_sprite = SpriteCache::load_sprite(def_shader, bullet_texture, bv, 4, "pbullet");
    //
    Sprite enemy_bullet_sprite = SpriteCache::load_sprite(def_shader, ebullet_texture, bv, 4, "ebullet");
    Sprite grunt_sprite = SpriteCache::load_sprite(def_shader, eship_texture, ev, 3, "grunt");
    Sprite snipe_sprite = SpriteCache::load_sprite(def_shader, snipe_texture, ev, 3, "snipe");
    Sprite hose_sprite = SpriteCache::load_sprite(def_shader, hose_texture, ev, 3, "hose");
    Sprite star_sprite = SpriteCache::load_sprite(def_shader, star_texture, ev, 3, "star");
    Sprite boss_sprite = SpriteCache::load_sprite(def_shader, star_texture, ev, 3, "boss");
    //

    auto text_system = GCR.register_system<TextSystem>(); // this could be a manager
    text_system->init(text_shader, ft, face);

    Game game;
    game.init();

    float dt = 0.0f;
    while (!quit) {
        auto start = (float) glfwGetTime();
        window_manager.process_events();
        game.loop(dt);
        text_system->update();
        window_manager.update();
        auto stop = (float) glfwGetTime();
        dt = stop - start;
    }
    window_manager.clean();
    return 0;
}
