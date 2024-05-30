#ifdef __EMSCRIPTEN__
    #include <emscripten.h>
    #include "../inc/emscripten_mainloop.h"
    #define GL_GLEXT_PROTOTYPES
    #define EGL_EGLEXT_PROTOTYPES
#else
    #include <glad.h>
#endif
#include <GLFW/glfw3.h>
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
#include "ui_manager.h"
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

#ifdef __EMSCRIPTEN__
    Shader text_shader = ResourceManager::load_shader("app/src/shaders/es/font.vert", "app/src/shaders/es/font.frag", "text");
    Shader def_shader = ResourceManager::load_shader("app/src/shaders/es/default.vert", "app/src/shaders/es/default.frag", "default");
#else
    Shader text_shader = ResourceManager::load_shader("shaders/font.vert", "shaders/font.frag", "text");
    Shader def_shader = ResourceManager::load_shader("shaders/default.vert", "shaders/default.frag", "default");
#endif
    UiManager ui_manager(SCR_WIDTH, SCR_HEIGHT, text_shader);

    Texture laser_texture = ResourceManager::load_texture("app/src/textures/laser.png", "laser", true);
    Texture bomb_texture = ResourceManager::load_texture("app/src/textures/bomb.png", "bomb", true);
    Texture bullet_texture = ResourceManager::load_texture("app/src/textures/bullet.png", "bullet", true);
    Texture ship_texture = ResourceManager::load_texture("app/src/textures/ship.png", "ship", true);
    Texture ebullet_texture = ResourceManager::load_texture("app/src/textures/ebullet.png", "ebullet", true);
    Texture eship_texture = ResourceManager::load_texture("app/src/textures/eship.png", "eship", true);
    Texture snipe_texture = ResourceManager::load_texture("app/src/textures/snipe.png", "snipe", true);
    Texture star_texture = ResourceManager::load_texture("app/src/textures/star.png", "star", true);
    Texture hose_texture = ResourceManager::load_texture("app/src/textures/hose.png", "hose", true);
    Texture boss_texture = ResourceManager::load_texture("app/src/textures/boss.png", "boss", true);
    Texture bgstar1_texture = ResourceManager::load_texture("app/src/textures/bgstar1.png", "bgstar1", true);
    Texture bgstar2_texture = ResourceManager::load_texture("app/src/textures/bgstar2.png", "bgstar2", true);
    Texture bgstar3_texture = ResourceManager::load_texture("app/src/textures/bgstar3.png", "bgstar3", true);

    SpriteCache::load_sprite(def_shader, ship_texture, v, 3, "player");
    SpriteCache::load_sprite(def_shader, laser_texture, lv, 4, "plaser");
    SpriteCache::load_sprite(def_shader, bomb_texture, bv, 4, "pbomb");
    SpriteCache::load_sprite(def_shader, bullet_texture, bv, 4, "pbullet");
    SpriteCache::load_sprite(def_shader, ebullet_texture, bv, 4, "ebullet");
    SpriteCache::load_sprite(def_shader, eship_texture, ev, 3, "grunt");
    SpriteCache::load_sprite(def_shader, snipe_texture, ev, 3, "snipe");
    SpriteCache::load_sprite(def_shader, hose_texture, ev, 3, "hose");
    SpriteCache::load_sprite(def_shader, star_texture, ev, 3, "star");
    SpriteCache::load_sprite(def_shader, boss_texture, ev, 3, "boss");
    SpriteCache::load_sprite(def_shader, bgstar1_texture, bv, 4, "bgstar1");
    SpriteCache::load_sprite(def_shader, bgstar2_texture, bv, 4, "bgstar2");
    SpriteCache::load_sprite(def_shader, bgstar3_texture, bv, 4, "bgstar3");

    Game game;

#ifdef __EMSCRIPTEN__
    float dt = 0.013f;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    float dt = 0.0f;
    while (!quit)
#endif
    {
#ifndef __EMSCRIPTEN__
        auto start = (float) glfwGetTime();
#endif
        window_manager.process_events();
        game.update(dt);
        ui_manager.update();
        window_manager.update();
        auto stop = (float) glfwGetTime();
#ifndef __EMSCRIPTEN__
        dt = stop - start;
#endif
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
    window_manager.clean();
    return 0;
}
