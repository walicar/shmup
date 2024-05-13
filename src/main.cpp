#include <glad.h>
#include <glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include "shader.h"
#include "resource_manager.h"
#include "window_manager.h"
#include "ecs/coordinator.h"
#include "ecs/types.h"
#include "components/sprite.h"
#include "components/velocity.h"
#include "components/hitbox.h"
#include "components/tags/player.h"
#include "components/tags/enemy.h"
#include "components/projectile.h"
#include "components/tags/controllable.h"
#include "components/transform.h"
#include "components/ai.h"
#include "systems/sprite_system.h"
#include "systems/movement_system.h"
#include "systems/projectile_system.h"
#include "systems/physics_system.h"
#include "systems/collision_system.h"
#include "systems/ai_system.h"
#include "systems/animation_system.h"
#include "src/components/state.h"
#include "src/systems/spawn_system.h"
#include <chrono>
#include <map>

Coordinator GCR;
static bool quit = false;

void handle_quit(Event &e) {
    quit = true;
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;

void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main() {
    // application entry
    WindowManager window_manager("SHMUP", 800, 600);
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

    // register components
    GCR.register_component<Sprite>();
    GCR.register_component<Transform>();
    GCR.register_component<Hitbox>();
    GCR.register_component<Velocity>();
    GCR.register_component<Player>();
    GCR.register_component<Enemy>();
    GCR.register_component<Projectile>();
    GCR.register_component<Controllable>();
    GCR.register_component<AI>();
    GCR.register_component<State>();

    // register systems
    /**
     * NOTE: any entity that has all components required by a system
     *  will be added to the that system's entity list
     */
    auto sprite_system = GCR.register_system<SpriteSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Sprite>());
        GCR.set_system_signature<SpriteSystem>(signature);
    }

    auto movement_system = GCR.register_system<MovementSystem>(); // rename to movement system
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Player>());
        signature.set(GCR.get_component_type<Controllable>());
        signature.set(GCR.get_component_type<Hitbox>());
        GCR.set_system_signature<MovementSystem>(signature);
    }

    movement_system->init();

    auto projectile_system = GCR.register_system<ProjectileSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Player>());
        signature.set(GCR.get_component_type<Sprite>());
        signature.set(GCR.get_component_type<Projectile>());
        GCR.set_system_signature<ProjectileSystem>(signature);
    }

    projectile_system->init();

    auto collision_system = GCR.register_system<CollisionSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Hitbox>());
        signature.set(GCR.get_component_type<Sprite>());
        signature.set(GCR.get_component_type<Transform>());
        GCR.set_system_signature<CollisionSystem>(signature);
    }

    collision_system->init();

    auto physics_system = GCR.register_system<PhysicsSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Velocity>());
        GCR.set_system_signature<PhysicsSystem>(signature);
    }

    physics_system->init();

    auto ai_system = GCR.register_system<AISystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Transform>());
        signature.set(GCR.get_component_type<Enemy>());
        signature.set(GCR.get_component_type<Sprite>());
        GCR.set_system_signature<AISystem>(signature);
    }

    ai_system->init();

    auto animation_system = GCR.register_system<AnimationSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Sprite>());
        GCR.set_system_signature<AnimationSystem>(signature);
    }

    animation_system->init();

    auto spawn_system = GCR.register_system<SpawnSystem>();
    {
        Signature signature;
        signature.set(GCR.get_component_type<Enemy>());
        GCR.set_system_signature<SpawnSystem>(signature);
    }

    spawn_system->init();

    // text

    if (FT_New_Face(ft, "fonts/font.ttf", 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
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
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // configure VAO/VBO for texture quads
    // -----------------------------------
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // create the player
    Entity player = GCR.create_entity();
    GCR.add_component(player, State{
        .active = true
    });
    GCR.add_component(player, Sprite{
            .shader = &def_shader,
            .texture = &ship_texture,
            .vertex_data = v,
            .vertex_count = 3 // @TODO: HARDCODED
    });
    GCR.add_component(player, Transform{});
    GCR.add_component(player, Player{});
    GCR.add_component(player, Controllable{});
    GCR.add_component(player, Hitbox{
            .health = 300,
            .hitbox = glm::vec3(0.05f, 0.05f, 0.05f)
    });
    auto &sprite = GCR.get_component<Sprite>(player);
    sprite.setup();

    // player projectiles

    // laser...
    Entity player_laser = GCR.create_entity();
    GCR.add_component(player_laser, State{});
    GCR.add_component(player_laser, Sprite{
            .shader = &def_shader,
            .texture = &laser_texture,
            .vertex_data = lv,
            .vertex_count = 4 // @TODO: HARDCODED
    });

    GCR.add_component(player_laser, Transform{
            .pos = glm::vec3(10.0f, 0.0f, 0.0f)
    });
    GCR.add_component(player_laser, Hitbox{
            .hitbox = glm::vec3(0.5f, 1.5f, 1.0f)
    });
    GCR.add_component(player_laser, Velocity{});
    GCR.add_component(player_laser, Player{});
    GCR.add_component(player_laser, Projectile{
        .damage = 100 // debug damage, set back to 1
    });
    auto &laser_sprite = GCR.get_component<Sprite>(player_laser);
    laser_sprite.setup();

    // bombs
    for (int i = 0; i < Entities::P_BOMB_AMT; i++) {
        Entity player_bomb = GCR.create_entity();
        GCR.add_component(player_bomb, State{});
        GCR.add_component(player_bomb, Sprite{
                .shader = &def_shader,
                .texture = &bomb_texture,
                .vertex_data = bv,
                .vertex_count = 4 // @TODO: HARDCODED
        });
        GCR.add_component(player_bomb, Transform{});
        GCR.add_component(player_bomb, Projectile{
            .type = BOMB,
        });
        GCR.add_component(player_bomb, Player{});
        GCR.add_component(player_bomb, Hitbox{
                .hitbox = glm::vec3(100.0f)
        });
        auto &bullet_sprite = GCR.get_component<Sprite>(player_bomb);
        bullet_sprite.setup();
    }

    // bullets
    for (int i = 0; i < Entities::P_BULLET_AMT; i++) {
        Entity player_bullet = GCR.create_entity();
        GCR.add_component(player_bullet, State{
                .active = false // redundant
        });
        GCR.add_component(player_bullet, Sprite{
                .shader = &def_shader,
                .texture = &bullet_texture,
                .vertex_data = bv,
                .vertex_count = 4 // @TODO: HARDCODED
        });

        GCR.add_component(player_bullet, Transform{});
        GCR.add_component(player_bullet, Player{});
        GCR.add_component(player_bullet, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 1.0f)
        });
        GCR.add_component(player_bullet, Velocity{});
        GCR.add_component(player_bullet, Projectile{
            .damage = 15
        });
        auto &bullet_sprite = GCR.get_component<Sprite>(player_bullet);
        bullet_sprite.setup();
    }

    // enemies

    // enemy grunts
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = true,
        });
        GCR.add_component(enemy, Sprite{
                .shader = &def_shader,
                .texture = &eship_texture,
                .vertex_data = ev,
                .vertex_count = 3 // @TODO: HARDCODED
        });
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
            .type = GRUNT
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 2.0f,
                .last_attacked = 0.0f + (1.0f * i)
        });
        GCR.add_component(enemy, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
        auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
        enemy_sprite.setup();
    }

    // snipe
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, Sprite{
                .shader = &def_shader,
                .texture = &snipe_texture,
                .vertex_data = ev,
                .vertex_count = 3 // @TODO: HARDCODED
        });
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
            .type = SNIPE
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 1.0f,
                .last_attacked = 0.0f + (1.0f * i)
        });
        GCR.add_component(enemy, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
        auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
        enemy_sprite.setup();
    }

    // hose
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, Sprite{
                .shader = &def_shader,
                .texture = &hose_texture,
                .vertex_data = ev,
                .vertex_count = 3 // @TODO: HARDCODED
        });
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
                .type = HOSE
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 0.75f,
                .last_attacked = 0.0f + (1.25f * i)
        });
        GCR.add_component(enemy, Hitbox{
            .health = 50,
            .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
        auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
        enemy_sprite.setup();
    }

    // star
    for (int i = 0; i < Entities::E_AMT; i++) {
        Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, Sprite{
                .shader = &def_shader,
                .texture = &star_texture,
                .vertex_data = ev,
                .vertex_count = 3 // @TODO: HARDCODED
        });
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
                .type = STAR
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 1.0f,
                .last_attacked = 0.0f + (0.25f * i)
        });
        GCR.add_component(enemy, Hitbox{
                .health = 50,
                .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
        auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
        enemy_sprite.setup();
    }

    for (int i = 0; i < Entities::E_AMT; i++) {
        if (i % 3 == 0) {
            Entity enemy = GCR.create_entity();
            GCR.add_component(enemy, State{
                    .active = false,
            });
            GCR.add_component(enemy, Sprite{
                    .shader = &def_shader,
                    .texture = &snipe_texture,
                    .vertex_data = ev,
                    .vertex_count = 3 // @TODO: HARDCODED
            });
            GCR.add_component(enemy, Transform{
                    .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                    .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
            });
            GCR.add_component(enemy, Enemy{
                    .type = SNIPE
            });
            GCR.add_component(enemy, AI{
                    .attack_cooldown = 3.0f,
                    .last_attacked = 0.0f + (1.0f * i)
            });
            GCR.add_component(enemy, Hitbox{
                    .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
            });
            auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
            enemy_sprite.setup();
        } else if (i % 3 == 1) {


            Entity enemy = GCR.create_entity();
            GCR.add_component(enemy, State{
                    .active = false,
            });
            GCR.add_component(enemy, Sprite{
                    .shader = &def_shader,
                    .texture = &hose_texture,
                    .vertex_data = ev,
                    .vertex_count = 3 // @TODO: HARDCODED
            });
            GCR.add_component(enemy, Transform{
                    .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                    .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
            });
            GCR.add_component(enemy, Enemy{
                    .type = HOSE
            });
            GCR.add_component(enemy, AI{
                    .attack_cooldown = 0.75f,
                    .last_attacked = 0.0f + (1.25f * i)
            });
            GCR.add_component(enemy, Hitbox{
                    .health = 50,
                    .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
            });
            auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
            enemy_sprite.setup();
        } else {
            Entity enemy = GCR.create_entity();
        GCR.add_component(enemy, State{
                .active = false,
        });
        GCR.add_component(enemy, Sprite{
                .shader = &def_shader,
                .texture = &star_texture,
                .vertex_data = ev,
                .vertex_count = 3 // @TODO: HARDCODED
        });
        GCR.add_component(enemy, Transform{
                .pos = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
                .origin = glm::vec3(-7.0f + (1.0f * i), 8.0f, 0.0f),
        });
        GCR.add_component(enemy, Enemy{
                .type = STAR
        });
        GCR.add_component(enemy, AI{
                .attack_cooldown = 1.0f,
                .last_attacked = 0.0f + (0.25f * i)
        });
        GCR.add_component(enemy, Hitbox{
                .health = 50,
                .hitbox = glm::vec3(0.5f, 0.5f, 0.5f)
        });
        auto &enemy_sprite = GCR.get_component<Sprite>(enemy);
        enemy_sprite.setup();
        }
    }

    // boss
    Entity boss = GCR.create_entity();
    GCR.add_component(boss, State{
            .active = false,
    });
    GCR.add_component(boss, Sprite{
            .shader = &def_shader,
            .texture = &boss_texture,
            .scale_factor = glm::vec3(0.22f),
            .vertex_data = ev,
            .vertex_count = 3 // @TODO: HARDCODED
    });
    GCR.add_component(boss, Transform{
            .pos = glm::vec3(0.0f, 4.0f, 0.0f),
            .origin = glm::vec3(0.0f, 4.0f, 0.0f),
    });
    GCR.add_component(boss, Enemy{
            .type = BOSS
    });
    GCR.add_component(boss, AI{
            .attack_cooldown = 0.17f,
    });
    GCR.add_component(boss, Hitbox{
            .health = 1000,
            .hitbox = glm::vec3(1.75f)
    });
    auto &boss_sprite = GCR.get_component<Sprite>(boss);
    boss_sprite.setup();
    std::cout << "BOSS ID ::: " << boss << std::endl;

    // enemy bullets
    for (int i = 0; i < Entities::E_BULLET_AMT; i++) {
        Entity enemy_bullet = GCR.create_entity();
        GCR.add_component(enemy_bullet, State{
                .active = false,
        });
        GCR.add_component(enemy_bullet, Sprite{
                .shader = &def_shader,
                .texture = &ebullet_texture,
                .vertex_data = bv,
                .vertex_count = 4 // @TODO: HARDCODED
        });

        GCR.add_component(enemy_bullet, Transform{
            .pos = glm::vec3(0.0f),
            .origin = glm::vec3(0.0f)
        });
        GCR.add_component(enemy_bullet, Enemy{});
        GCR.add_component(enemy_bullet, Hitbox{
                .hitbox = glm::vec3(0.5f, 0.5f, 1.0f)
        });
        GCR.add_component(enemy_bullet, Velocity{});
        GCR.add_component(enemy_bullet, Projectile{});
        auto &bullet_sprite = GCR.get_component<Sprite>(enemy_bullet);
        bullet_sprite.setup();
    }

    float dt = 0.0f;

    while (!quit) {
        auto start = (float) glfwGetTime();
        window_manager.process_events();
        projectile_system->update((float) glfwGetTime());
        movement_system->update(dt);
        ai_system->update((float) glfwGetTime());
        collision_system->update(dt);
        physics_system->update(dt);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sprite_system->update((float) glfwGetTime());
        animation_system->update((float) glfwGetTime());
        spawn_system->update();
        //text_system->update();
        RenderText(text_shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        RenderText(text_shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
        window_manager.update();
        auto stop = (float) glfwGetTime();
        dt = stop - start;

    }
    window_manager.clean();
    return 0;
}
